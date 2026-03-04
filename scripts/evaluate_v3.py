import os
import pandas as pd
from stable_baselines3.common.vec_env import DummyVecEnv, VecNormalize
from stable_baselines3.common.monitor import Monitor

from reev_control.envs import SimpleVehicleEnv2, SimpleVehicleEnv3
from reev_control.envs.wrappers import ActionFlatteningWrapper, InfoSumWrapper
from reev_control.custom_ppo import CustomPPO  # your CustomPPO
import torch
import yaml

# --- Settings ---
run_name = "offline-run-20250613_165352-4y1d388m"  # name of the run to evaluate
folder = f"train_results/wandb/{run_name}/files"

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

train_config['file_list_file'] = "data/train/Mar2025_filtered_files.pkl"  # path to file list


# obs_keys_to_log = ["some_obs_name1", "some_obs_name2"]  # update based on your actual obs
# info_keys_to_log = ["nvh_reward_sum", "efficiency_reward_sum", "step_soc_reward_sum", "end_soc_reward"]  # update accordingly

# --- Create single environment for inference ---
def make_eval_env(seed: int = 0, **kwargs):

    def _init():

        env = SimpleVehicleEnv3(
            data_folder='data/train/REEV07RearDrive_Mar2025',
            seed=seed,
            **kwargs)

        return env

    return _init

env = DummyVecEnv([make_eval_env(**train_config)])




# --- Load VecNormalize ---
env = VecNormalize.load(vecnorm_path, env)
env.training = False
env.norm_reward = False     # want original reward in eval
obs_names = ["obs." + name for name in env.envs[0]._get_obs_names()]

# --- Load model ---
model = CustomPPO.load(model_path,
                       env=env,     # not sure if need to pass in env here
                       device='cpu'
                       )
model.policy.eval()

# --- Inference Loop ---
output_dir = f"eval_results/{run_name}"
os.makedirs(output_dir, exist_ok=True)
num_rollouts = 20

for i in range(num_rollouts):
    obs = env.reset()
    done = False
    log = []
    step = 0

    file_name = os.path.basename(env.envs[0].trajectory_loader.file_name).split('.')[0]

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
            "reward": original_reward[0],
            "done": done[0],
            **obs_data,
            **info_data
        })

        obs = next_obs
        step += 1

    # --- Save to DataFrame ---
    df = pd.DataFrame(log)
    save_path = os.path.join(output_dir, f"rollout_{file_name}.csv")
    df.to_csv(save_path, index=False)
    print(f"Saved inference log to {save_path}")
