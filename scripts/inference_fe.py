import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from stable_baselines3.common.vec_env import DummyVecEnv, VecNormalize
from reev_control.envs import SimpleVehicleEnv4FE
from reev_control.envs.wrappers import InfoHistoryWrapper
from reev_control.custom_ppo import CustomPPO
from reev_control.common.feature_extractor import LSTMFeatureExtractor

from sb3_ppo_train_fe import make_env

# === CONFIGURATION ===
run_name = "mh45qn8k"
MODEL_PATH = f"train_results/models/{run_name}/model.zip"  # Update with your model path
VEC_NORM_PATH = f"train_results/models/{run_name}/vec_env.pkl"     # Path to saved VecNormalize

# MODEL_PATH = f"/data2/qingyuan/projects/zengcheng3/train_results/models/if4qereu/model.zip"  # Update with your model path
# VEC_NORM_PATH = f"/data2/qingyuan/projects/zengcheng3/train_results/models/mh45qn8k/checkpoints/ppo_vecnormalize_2257664_steps.pkl"     # Path to saved VecNormalize

OUTPUT_CSV =  f"train_results/models/{run_name}/sample_inference_result.csv"
# PLOT_OUTPUT = f"train_results/models/{run_name}/sample_inference_plot.png"

# Set the data you want to use (one file from your dataset)
DATA_FOLDER = "data/train/REEV07RearDrive_Mar2025"

ENV_CONFIG = {
    "config_path": "reev_control/envs/config.yaml",
    "obs_seq_len": 600,  # in seconds, = 10 minutes
    "data_start_index": 600,
    "data_min_length": 3600,
    "step_size_in_seconds": 10,
    "reward_weights": [1, 5, 0.1, 0.05],
    "file_list_file": "data/train/Mar2025_filtered_files.pkl"  # pickle file with list of files to load, if None, will load all files in data_folder
}



# === MAIN INFERENCE SCRIPT ===
if __name__ == "__main__":
    os.makedirs(os.path.dirname(OUTPUT_CSV), exist_ok=True)

    print("🚀 Starting single-trajectory inference...")

    # === 1. Create the base environment (single instance) ===
    env = DummyVecEnv([make_env(**ENV_CONFIG)])

    vecnorm_env = VecNormalize.load(VEC_NORM_PATH, env)

    vecnorm_env.training = False
    # vecnorm_env.norm_obs = True
    # vecnorm_env.norm_reward = False

    # === 4. Load the trained model ===
    model = CustomPPO.load(
        MODEL_PATH,
        env=vecnorm_env,
        device="cuda:5"  # Or "cpu" if needed
    )

    # === 5. Run inference on one trajectory ===
    obs = vecnorm_env.reset()  # Reset and normalize
    done = False
    episode_infos = []  # Hold all per-step info

    print("📥 Starting episode loop...")


    # LSTM hidden state in model predict
    state = None
    episode_start = np.array([True])  # True at the first step

    while not done:
        action, state = model.predict(
            obs,
            state=state,
            episode_start=episode_start,
            deterministic=False,
        )
        obs, reward, done, info = vecnorm_env.step(action)
        episode_start = np.array([done])

        episode_infos.append(info[0])        # Extract info for this step

    print(f"✅ Done. Collected {len(episode_infos)} steps.")

    # === 6. Save to CSV ===
    df = pd.DataFrame(episode_infos)
    print("max power request: ", df['action.power_request'].max())
    df.to_csv(OUTPUT_CSV, index=False)
    print(f"💾 Data saved to: {OUTPUT_CSV}")
