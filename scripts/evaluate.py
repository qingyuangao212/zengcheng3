import os
import pandas as pd
from stable_baselines3.common.vec_env import DummyVecEnv, VecNormalize
from stable_baselines3.common.monitor import Monitor

from reev_control.envs import SimpleVehicleEnv2
from reev_control.envs.wrappers import ActionFlatteningWrapper, InfoSumWrapper
from reev_control.custom_ppo import CustomPPO  # your CustomPPO
from reev_control.envs.utils import CLASS_TO_ENV  
import torch
import yaml

# --- Settings ---
folder = "train_results/wandb/offline-run-20250603_171003-2v1jr8zy/files"
model_path = f"{folder}/model.zip"
vecnorm_path = f"{folder}/vec_env.pkl"  # adjust path

# load wandb config from train folder
with open(f'{folder}/config.yaml', 'r') as file:
    train_config = yaml.safe_load(file)

train_config = {
    k: v["value"]
    for k, v in train_config.items()
    if (isinstance(v, dict) and "value" in v and k!='_wandb')
}


# obs_keys_to_log = ["some_obs_name1", "some_obs_name2"]  # update based on your actual obs
# info_keys_to_log = ["nvh_reward_sum", "efficiency_reward_sum", "step_soc_reward_sum", "end_soc_reward"]  # update accordingly




# --- Create single environment for inference ---
def make_eval_env(seed: int = 0, env_class: str = "SimpleVehicleEnv2", **kwargs):

    def _init():

        env = CLASS_TO_ENV[env_class](
            data_folder='data/train/REEV07RearDrive_Mar2025',
            seed=seed,
            **kwargs)
        # env.reset()
        return env

    return _init

env = DummyVecEnv([make_eval_env(**train_config)])




# --- Load VecNormalize ---
env = VecNormalize.load(vecnorm_path, env)
env.training = False
env.norm_reward = False     # want original reward in eval
obs_names = ["obs." + name for name in env.unwrapped[0]._get_obs_names]

# --- Load model ---
model = CustomPPO.load(model_path, 
                       env=env,     # not sure if need to pass in env here
                       device='cpu'
                       )

model.policy.eval()

# --- Inference Loop ---

obs = env.reset()
done = False
log = []
step = 0

while not done:
    action, _ = model.predict(obs, deterministic=True)
    next_obs, reward, done, info = env.step(action)

    original_obs = env.get_original_obs()
    original_reward = env.get_original_reward()
    obs_array = original_obs[0]  # use unnormalized obs

    obs_data = dict(zip(obs_names, obs_array))

    info_data = {f"info.{k}": info[0].get(k, None) for k in info[0].keys()}

    log.append({
        "step": step,
        "action": action[0],
        "reward": original_reward[0],
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
