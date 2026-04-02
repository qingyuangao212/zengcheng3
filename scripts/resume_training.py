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

from sb3_ppo_train_env4 import make_env



# ======================================================
# Main
# ======================================================
if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Train or resume PPO model for reev_control")
    parser.add_argument("--run", type=str, required=True)
    parser.add_argument("--device", type=str, required=False, default="cpu", help="Device to train on (e.g., 'cpu', 'cuda:0', etc.)")
    args = parser.parse_args()

    # ======================================================
    # Resume configuration
    # ======================================================
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
    # Load config from wandb run
    # --------------------------------------------------
    TRAIN_CONFIG = run.config["TRAIN_CONFIG"]
    ENV_CONFIG = run.config["ENV_CONFIG"]

    # --------------------------------------------------
    # Rebuild vectorized environment
    # --------------------------------------------------
    vec_env = SubprocVecEnv(
        [
            make_env(seed=TRAIN_CONFIG["seed"] + i, **ENV_CONFIG)
            for i in range(TRAIN_CONFIG["n_envs"])
        ]
    )

    

    print(f"[RESUME] vec_env.observation_space = {vec_env.observation_space}")

    # --------------------------------------------------
    # Load VecNormalize (MUST be before model load)
    # --------------------------------------------------
    vec_env = VecNormalize.load(
        VECNORM_PATH,
        vec_env,
    )

    vec_env.training = True
    vec_env.norm_reward = TRAIN_CONFIG["normalize_reward"]

    # --------------------------------------------------
    # Load model
    # --------------------------------------------------
    model = CustomPPO.load(
        MODEL_PATH,
        env=vec_env,
        device=args.device if args.device else TRAIN_CONFIG["device"],
    )

    print(f"[RESUME] model.num_timesteps = {model.num_timesteps:,}")

    # --------------------------------------------------
    # Resume training
    # --------------------------------------------------

    print(f"[RESUME] current_timesteps = {model.num_timesteps:,}, target_total = {TRAIN_CONFIG['total_timesteps']:,}")

    model.learn(
        total_timesteps=TRAIN_CONFIG["total_timesteps"],
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
