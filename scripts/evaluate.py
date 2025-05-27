
import os
import pandas as pd
from stable_baselines3.common.vec_env import DummyVecEnv, VecNormalize
from stable_baselines3.common.monitor import Monitor

from reev_control.envs import SimpleVehicleEnv2
from reev_control.envs.wrappers import ActionFlatteningWrapper, InfoSumWrapper
from reev_control.custom_ppo import CustomPPO  # your CustomPPO
import torch

# --- Settings ---
model_path = "train_results/wandb/run-20250523_173837-czln0k89/files/model.zip"
vecnorm_path = "train_results/models/<run_id>/vecnormalize.pkl"  # adjust path
obs_keys_to_log = ["some_obs_name1", "some_obs_name2"]  # update based on your actual obs
info_keys_to_log = ["nvh_reward_sum", "efficiency_reward_sum", "step_soc_reward_sum", "end_soc_reward"]  # update accordingly

# --- Create single environment for inference ---
def make_single_env():
    env = SimpleVehicleEnv2(
        data_folder='data/train/REEV07RearDrive_Mar2025',
        config_path="reev_control/envs/config.yaml",
        obs_seq_len=1800,
        data_start_index=600,
        data_min_length=1800,
        step_size_in_seconds=30,
        reward_weights=[0.001, 20, 0.001, 0.05],
        seed=42,
    )
    env = ActionFlatteningWrapper(env)
    env = InfoSumWrapper(env, info_keys=["nvh_reward", "efficiency_reward", "step_soc_reward", "action_norm"])
    env = Monitor(env, info_keywords=info_keys_to_log)
    return env

env = DummyVecEnv([make_single_env])

# --- Load VecNormalize ---
env = VecNormalize.load(vecnorm_path, env)
env.training = False
env.norm_reward = False

# --- Load model ---
model = CustomPPO.load(model_path, env=env, device='cpu')

# --- Inference Loop ---
obs = env.reset()
done = False
log = []
step = 0

while not done:
    action, _ = model.predict(obs, deterministic=True)
    next_obs, reward, done, info = env.step(action)

    obs_array = obs[0]
    obs_data = {}
    for i, key in enumerate(obs_keys_to_log):
        obs_data[f"observation.{key}"] = obs_array[i] if i < len(obs_array) else None

    info_data = {f"info.{k}": info[0].get(k, None) for k in info_keys_to_log}

    log.append({
        "step": step,
        "action": action[0],
        "reward": reward[0],
        "done": done[0],
        **obs_data,
        **info_data
    })

    obs = next_obs
    step += 1

# --- Save to DataFrame ---
df = pd.DataFrame(log)
df.to_csv("inference_log.csv", index=False)
print("Saved inference log to inference_log.csv")
