import os

from stable_baselines3.common.vec_env import SubprocVecEnv, VecNormalize
from stable_baselines3.common.callbacks import CallbackList
from stable_baselines3.common.monitor import Monitor

import wandb
from wandb.integration.sb3 import WandbCallback

from reev_control.envs import SimpleVehicleEnv5
from reev_control.envs.wrappers import InfoSumWrapper, InfoHistoryWrapper
from reev_control.custom_ppo import CustomPPO
from reev_control.common.lr_schedule import linear_schedule
from reev_control.common.callbacks import WandbCallbackWithVecNorm, AdvantageLoggingCallback
from reev_control.common.feature_extractor import LSTMFeatureExtractor
from stable_baselines3.common.callbacks import CheckpointCallback

import argparse

info_keys = ["fc_reward", "efficiency_reward", "step_soc_reward", "action.engine_stop", "action.power_request"]
logged_info_keys = [key + '_sum' for key in info_keys] + [key + '_avg' for key in info_keys] + ["end_soc_reward"] # end_soc_reward is not summed, but logged at the end of episode


def make_env(seed: int | None =None, **kwargs):

    def _init():

        env = SimpleVehicleEnv5(data_folder='data/train/REEV07RearDrive_Mar2025',
                            seed=seed, **kwargs)
        env = InfoHistoryWrapper(env, info_keys=info_keys)  # sum all step info values to episode end info
        env = Monitor(env, info_keywords=logged_info_keys)  # update info['episode'] with info_keys, when gets sent to ep_info_buffer
        env.reset()  # not sure if vec env will still call reset again (can add print under env.reset to check)
        return env

    return _init

env_config = {
    # "env_class": "SimpleVehicleEnv5FE",  # simplified action space
    "config_path": "reev_control/envs/config.yaml",
    "obs_seq_len": 600,  # in seconds, = 10 minutes
    "data_start_index": 600,
    "data_min_length": 3600,
    "step_size_in_seconds": 10,
    "reward_weights": [1, 1, 0.1, 0.05],
    "file_list_file": "data/train/Mar2025_filtered_files.pkl"  # pickle file with list of files to load, if None, will load all files in data_folder
}

train_config = {
    "n_envs": 8,  # number of parallel environments
    "policy_type": "MlpPolicy",
    "total_timesteps": 5_000_000,
    "n_steps": 512,  # number of steps to run per environment per rollout
    "batch_size": 256,
    "n_epochs": 10,
    "gamma": 0.98,
    "gae_lambda": 0.98,
    "learning_rate": 3e-4,
    "ent_coef": 0.05,
    "vf_coef": 0.25,
    "device": "cpu",
    "vecnorm_gamma": 0.95
}

config = {**env_config, **train_config}

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Train or resume PPO model for reev_control")
    parser.add_argument("--run", type=str, required=False, help="run name")
    parser.add_argument("--notes", type=str, required=False, help="notes to add to wandb run")
    parser.add_argument("--device", type=str, required=False, help="Device to train on (e.g., 'cpu', 'cuda:0', etc.)")
    args = parser.parse_args()

    if args.device:
        train_config['device'] = args.device

    # os.environ['WANDB_INIT_TIMEOUT'] = '300'
    os.environ["WANDB_DIR"] = "train_results"

    run_name = "PPO_env5_20260303"
    if args.run:
        run_name = args.run

    run_notes = """
    env5: Tuple action space (engine_stop, power)
    """
    if args.notes:
        run_notes = args.notes


    # init wandb
    run = wandb.init(
        project="reev_control",
        name=run_name,
        config=config,
        sync_tensorboard=True,
        monitor_gym=True,
        save_code=True,
        notes="""
        initial_soc 30-80
        env5: Tuple action space (engine_stop, power)
        """
    )

    train_config['run_id'] = run.id

    # ==============Environment Setup=================
    # init vectorized environment
    vec_env = SubprocVecEnv([
        make_env(seed=100 + i, **env_config)
        for i in range(train_config["n_envs"])
    ])

    # VecNormalize if configured, otherwise create new VecNormalize
    if 'vecnorm_load_path' in train_config:
        vec_env = VecNormalize.load(train_config['vecnorm_load_path'], vec_env)
    else:
        vec_env = VecNormalize(vec_env,
                               training=True,
                               norm_obs=True,
                               norm_reward=False,
                            #    clip_obs=10.0,
                            #    clip_reward=15.0,\
                            )

    # ==============Model Setup=================
    # load model if configured, otherwise create new model (model loading should pair vecnorm loading)
    if 'model_load_path' in train_config:
        model = CustomPPO.load(train_config['model_load_path'],
                               env=vec_env,
                               device=train_config['device'])
    else:
        model = CustomPPO(
            policy=train_config['policy_type'],
            env=vec_env,
            verbose=1,
            device=train_config['device'],
            n_steps=train_config['n_steps'],
            batch_size=wandb.config.batch_size,
            n_epochs=wandb.config.n_epochs,
            gamma=train_config['gamma'],
            gae_lambda=train_config['gae_lambda'],
            learning_rate=linear_schedule(train_config['learning_rate']),
            ent_coef=train_config['ent_coef'],
            vf_coef=train_config['vf_coef'],
            tensorboard_log=f"train_results/tensorboard/{run.id}",
            info_keys=logged_info_keys,
            use_sde=True,
            policy_kwargs=dict(squash_output=True)      # last two lines for applying tanh to action output and transform to (-1,1) then scale to action space
        )

    model.learn(

        total_timesteps=train_config['total_timesteps'],
        callback=CallbackList([
            # AdvantageLoggingCallback(),
            WandbCallbackWithVecNorm(
                gradient_save_freq=100,
                model_save_path=f"train_results/models/{run.id}",
                model_save_freq=10_000,
                verbose=2),

            CheckpointCallback(
                save_freq=10_000,
                save_path=f"train_results/models/{run.id}/checkpoints/",
                name_prefix="ppo",
                save_vecnormalize=True)
        ]),
        log_interval=1
    )

    # # Optionally, evaluate
    # obs = vec_env.reset()
    # for _ in range(1000):
    #     action, _states = model.predict(obs)
    #     obs, rewards, dones, infos = vec_env.step(action)

    # vec_env.close()
    run.finish()

# TBD
