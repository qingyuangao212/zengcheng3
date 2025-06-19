import os

from stable_baselines3.common.vec_env import SubprocVecEnv, VecNormalize
from stable_baselines3.common.callbacks import CallbackList
from stable_baselines3.common.monitor import Monitor

import wandb
from wandb.integration.sb3 import WandbCallback

from reev_control.envs import SimpleVehicleEnv3FE
from reev_control.envs.wrappers import ActionFlatteningWrapper, InfoSumWrapper, InfoHistoryWrapper
from reev_control.custom_ppo import CustomPPO
from reev_control.common.lr_schedule import linear_schedule
from reev_control.common.callbacks import WandbCallbackWithVecNorm
from reev_control.common.feature_extractor import LSTMFeatureExtractor


sum_info_keys = ["nvh_reward", "efficiency_reward", "step_soc_reward", "action.engine_stop", "action.power_request"]  
# sum_info_keys = ["nvh_reward", "efficiency_reward", "step_soc_reward", "action_norm" ]   # for env1
logged_info_keys = [key + '_sum' for key in sum_info_keys] + [key + '_avg' for key in sum_info_keys] + ["end_soc_reward"] # end_soc_reward is not summed, but logged at the end of episode


def make_env(seed: int = 0, **kwargs):

    def _init():

        env = SimpleVehicleEnv3FE(data_folder='data/train/REEV07RearDrive_Mar2025',
                            seed=seed, **kwargs)
        env = InfoHistoryWrapper(env, info_keys=sum_info_keys)  # sum all step info values to episode end info
        env = Monitor(env, info_keywords=logged_info_keys)  # update info['episode'] with info_keys, when gets sent to ep_info_buffer
        env.reset()  # not sure if vec env will still call reset again (can add print under env.reset to check)
        return env

    return _init


if __name__ == "__main__":

    # os.environ['WANDB_INIT_TIMEOUT'] = '300'
    os.environ["WANDB_DIR"] = "train_results"


    env_config = {
        "env_class": "SimpleVehicleEnv3FE",  # simplified action space
        "config_path": "reev_control/envs/config.yaml",
        # "obs_seq_len": 600,  # in seconds, = 10 minutes
        "obs_seq_len": 1800,  # in seconds, = 30 minutes
        "data_start_index": 600,
        "data_min_length": 3600,
        "step_size_in_seconds": 30,
        # "reward_weights": [0.001, 3.3, 0.0005, 0.05], 
        "reward_weights": [0.001, 3.3, 0.05, 0.05], 
        "file_list_file": "data/train/Mar2025_filtered_files.pkl" # pickle file with list of files to load, if None, will load all files in data_folder
    }

    # info = {"nvh_reward": weights[0]*r_nvh,
    #         "efficiency_reward": weights[1]*r_efficiency,
    #         "step_soc_reward": weights[2]*r_step_soc,
    #         "end_soc_reward": weights[3]*r_end_soc}

    train_config = {
        "n_envs": 16,  # number of parallel environments
        "policy_type": "MlpPolicy",
        "total_timesteps": 50_000_000,
        "n_steps": 1024,  # number of steps to run per environment per rollout
        "batch_size": 512, 
        "n_epochs": 10,
        "gamma": 0.95,   
        "learning_rate": 3e-4,
        "ent_coef": 0.05,
        "vf_coef": 0.5,
        "device": 'cpu',
        # "model_load_path": "train_results/wandb/run-20250523_173837-czln0k89/files/model.zip"     # uaw this together with vecnorm_load_path
        # "vecnorm_load_path": ...
    }

    config = {**env_config, **train_config}

    # init wandb
    run = wandb.init(
        project="reev_control",
        name="PPO_0618_env3FE",
        config=config,
        sync_tensorboard=True,
        monitor_gym=True,
        save_code=True,
    )

    run.notes = "previous run action converges to power 3kw or 99% stop engine, and in evaluation SOC goes<0; add stronger step_soc reward, tune params (lower gamma, increase entropy bonus)"

    train_config['run_id'] = run.id

    # ==============Environment Setup================= 
    # init vectorized environment
    vec_env = SubprocVecEnv([
        make_env(seed=100 + i, **env_config) for i in range(train_config["n_envs"])
    ])


    # ==============Model Setup=================
    # load model if configured, otherwise create new model (model loading should pair vecnorm loading)
    if 'model_load_path' in train_config:
        model = CustomPPO.load(train_config['model_load_path'],
                               env=vec_env,
                               device=train_config['device'])
    else:
        model = CustomPPO(
            policy="MultiInputPolicy",
            env=vec_env,
            verbose=1,
            device=train_config['device'],
            n_steps=train_config['n_steps'],
            batch_size=wandb.config.batch_size,
            n_epochs=wandb.config.n_epochs,
            gamma=wandb.config.gamma,
            learning_rate=linear_schedule(train_config['learning_rate']),
            ent_coef=train_config['ent_coef'],
            vf_coef=train_config['vf_coef'],
            tensorboard_log=f"train_results/tensorboard/{run.id}",
            info_keys=logged_info_keys,
            policy_kwargs=dict(features_extractor_class=LSTMFeatureExtractor)  # use LSTM feature extractor for MlpPolicy
            )



    model.learn(
        total_timesteps=wandb.config.total_timesteps,
        callback=CallbackList([
            WandbCallbackWithVecNorm(
                gradient_save_freq=100,
                model_save_path=f"train_results/models/{run.id}",
                model_save_freq=10 * train_config['n_steps'],  # this is freq for rollout steps: on_step. set it equal to n_steps to save model every rollout call
                verbose=2)
                ]),
        log_interval=1)

    # # Optionally, evaluate
    # obs = vec_env.reset()
    # for _ in range(1000):
    #     action, _states = model.predict(obs)
    #     obs, rewards, dones, infos = vec_env.step(action)

    # vec_env.close()
    run.finish()

# TBD
