import os

import wandb
import argparse

from stable_baselines3.common.vec_env import SubprocVecEnv, VecNormalize
from stable_baselines3.common.callbacks import CallbackList, CheckpointCallback

from reev_control.custom_ppo import CustomPPO
from reev_control.common.callbacks import (
    WandbCallbackWithVecNorm,
    AdvantageLoggingCallback,
)

from sb3_ppo_train_env4 import *



# ======================================================
# Main
# ======================================================
if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Train or resume PPO model for reev_control")
    parser.add_argument("--run", type=str, required=False)
    parser.add_argument("--device", type=str, required=False, default="cuda:5", help="Device to train on (e.g., 'cpu', 'cuda:0', etc.)")
    args = parser.parse_args()

    # ======================================================
    # Resume configuration
    # ======================================================
    RESUME_RUN_ID = "mh45qn8k"
    if args.run:
        RESUME_RUN_ID = args.run

    MODEL_PATH = f"train_results/models/{RESUME_RUN_ID}/model.zip"
    VECNORM_PATH = f"train_results/models/{RESUME_RUN_ID}/vec_env.pkl"

    # env_config.update({"reward_weights": [1, 5, 0.1, 0.05]})


    os.environ["WANDB_DIR"] = "train_results"

    # --------------------------------------------------
    # Resume W&B run
    # --------------------------------------------------
    run = wandb.init(
        project="reev_control",
        id=RESUME_RUN_ID,
        resume="must",
        sync_tensorboard=True,
        monitor_gym=True,
        save_code=True,
    )

    # --------------------------------------------------
    # Rebuild vectorized environment
    # --------------------------------------------------
    vec_env = SubprocVecEnv(
        [
            make_env(seed=200 + i, **env_config)
            for i in range(train_config["n_envs"])
        ]
    )

    # --------------------------------------------------
    # Load VecNormalize (MUST be before model load)
    # --------------------------------------------------
    vec_env = VecNormalize.load(
        VECNORM_PATH,
        vec_env,
    )

    vec_env.training = True
    vec_env.norm_reward = False

    # --------------------------------------------------
    # Load model
    # --------------------------------------------------
    model = CustomPPO.load(
        MODEL_PATH,
        env=vec_env,
        device=args.device if args.device else train_config["device"],
    )

    print(f"[RESUME] model.num_timesteps = {model.num_timesteps:,}")

    # --------------------------------------------------
    # Optional: train only until total = total_timesteps
    # --------------------------------------------------
    remaining_steps = max(
        train_config["total_timesteps"] - model.num_timesteps,
        0,
    )

    print(f"[RESUME] remaining_steps = {remaining_steps:,}")

    if remaining_steps == 0:
        print("Nothing to train. Exiting.")
        run.finish()
        exit(0)

    # --------------------------------------------------
    # Resume training
    # --------------------------------------------------
    model.learn(
        total_timesteps=remaining_steps,
        reset_num_timesteps=False,   # 🔥 DO NOT REMOVE
        callback=CallbackList(
            [
                # AdvantageLoggingCallback(),

                WandbCallbackWithVecNorm(
                    gradient_save_freq=100,
                    model_save_path=f"train_results/models/{run.id}",
                    model_save_freq=10_000,
                    verbose=2,
                ),

                CheckpointCallback(
                    save_freq=10_000,
                    save_path=f"train_results/models/{run.id}/checkpoints/",
                    name_prefix="ppo",
                    save_vecnormalize=True,
                ),
            ]
        ),
        log_interval=1,
    )

    run.finish()
