from typing import Optional
import gymnasium as gym
import wandb
import numpy as np

from gymnasium.wrappers import TimeLimit

# from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import SubprocVecEnv, VecNormalize
# from stable_baselines3.common.utils import set_random_seed
# from stable_baselines3.common.callbacks import BaseCallback
from stable_baselines3.common.monitor import Monitor

from wandb.integration.sb3 import WandbCallback

from reev_control.envs import SimpleVehicleEnv, SimpleVehicleEnv2
from reev_control.envs.wrappers import ActionFlatteningWrapper, InfoSumWrapper
from reev_control.custom_ppo import CustomPPO
from reev_control.common.lr_schedule import linear_schedule




sum_info_keys = ["nvh_reward", "efficiency_reward", "step_soc_reward", "action_norm"]   # throw these into InfoSumWrapper
logged_info_keys = [key + '_sum' for key in sum_info_keys] + [key + '_avg' for key in sum_info_keys] + ["end_soc_reward"] # end_soc_reward is not summed, but logged at the end of episode

CLASS_TO_ENV = {
    "SimpleVehicleEnv": SimpleVehicleEnv,
    "SimpleVehicleEnv2": SimpleVehicleEnv2
}

def make_env(seed: int = 0, env_class: str = "SimpleVehicleEnv", **kwargs):

    def _init():

        env = CLASS_TO_ENV[env_class](data_folder='data/train/REEV07RearDrive_Mar2025',
                              seed=seed,
                              **kwargs)

        if env_class == "SimpleVehicleEnv":
            env = ActionFlatteningWrapper(env)

        env = InfoSumWrapper(env, info_keys=sum_info_keys
                             )  # sum all step info values to episode end info
        
        env = Monitor(
            env, info_keywords=logged_info_keys
        )  # update info['episode'] with info_keys, when gets sent to ep_info_buffer

        env.reset(
        )  # not sure if vec env will still call reset again (can add print under env.reset to check)
        return env

    return _init


if __name__ == "__main__":

    import os
    from stable_baselines3.common.callbacks import CallbackList

    os.environ["WANDB_DIR"] = "train_results"

    env_config = {
        "env_class": "SimpleVehicleEnv2",   # simplified action space
        "config_path": "reev_control/envs/config.yaml",
        # "obs_seq_len": 600,  # in seconds, = 10 minutes
        "obs_seq_len": 1800,  # in seconds, = 10 minutes
        "data_start_index": 600,
        "data_min_length": 1800,
        "step_size_in_seconds": 30,
        # "reward_weights":  [1e-5, 0.5e-2, 1e-6, 1]
        "reward_weights":  [0.001, 20, 0.001, 0.05]
    }


    # info = {"nvh_reward": weights[0]*r_nvh,
    #         "efficiency_reward": weights[1]*r_efficiency,
    #         "step_soc_reward": weights[2]*r_step_soc,
    #         "end_soc_reward": weights[3]*r_end_soc}

    train_config = {
            "n_envs": 16,  # number of parallel environments
            "policy_type": "MlpPolicy",
            "total_timesteps": 10000_000,
            "n_steps": 1024,    # number of steps to run per environment per rollout
            "batch_size": 512,
            "n_epochs": 10,
            "gamma": 0.99,
            "learning_rate": 3e-4,
            "ent_coef": 0.01,
            "device": 'cpu'
        }

    config = {**env_config, **train_config}

    # Initialize Weights & Biases
    run = wandb.init(
        project="reev_control",
        name="PPO_REEV07_experiment_0523_v2",
        notes="SimpleVehicleEnv2/March data/vec normalize: verify if this affects reward logging (note this effectively wipes out the large end_soc condition, for future consider smoothing it out to previous steps)",
        config=config,
        sync_tensorboard=True,
        monitor_gym=True,
        save_code=True,
    )

    seed = 100
    vec_env = SubprocVecEnv([make_env(seed=seed+i, **env_config) for i in range(train_config["n_envs"])])

    vec_env = VecNormalize(vec_env, 
                           training=True, 
                           norm_obs=True, 
                           norm_reward=True, 
                           clip_obs=10.0, 
                           clip_reward=10.0, 
                           gamma=0.99, 
                           epsilon=1e-08, 
                           norm_obs_keys=None)
        
    model = CustomPPO(
        policy="MlpPolicy",
        env=vec_env,
        verbose=1,
        device=train_config['device'],
        n_steps=train_config['n_steps'],
        batch_size=wandb.config.batch_size,
        n_epochs=wandb.config.n_epochs,
        gamma=wandb.config.gamma,
        # learning_rate=wandb.config.learning_rate,
        learning_rate=linear_schedule(train_config['learning_rate']),
        ent_coef=wandb.config.ent_coef,
        tensorboard_log=f"train_results/tensorboard/{run.id}",
        info_keys=logged_info_keys
    )

    model.learn(
        total_timesteps=wandb.config.total_timesteps,
        callback=CallbackList([
            WandbCallback(gradient_save_freq=100,
                          model_save_path=f"train_results/models/{run.id}",
                          model_save_freq=10*train_config['n_steps'], # this is freq for rollout steps: on_step. set it equal to n_steps to save model every rollout call
                          verbose=2),
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
