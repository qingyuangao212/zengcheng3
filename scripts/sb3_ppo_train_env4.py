"""
PPO training script for REEV control using SimpleVehicleEnv4.
"""

import argparse
import datetime
import os
import random
import uuid

import wandb
from stable_baselines3.common.callbacks import CallbackList, CheckpointCallback
from stable_baselines3.common.monitor import Monitor
from stable_baselines3.common.vec_env import SubprocVecEnv, VecNormalize

from reev_control.custom_ppo import CustomPPO
from reev_control.common.callbacks import WandbCallbackWithVecNorm
from reev_control.common.lr_schedule import linear_schedule
from reev_control.envs import SimpleVehicleEnv4
from reev_control.envs.wrappers import InfoHistoryWrapper


# ==============================================================================
# Configuration
# ==============================================================================

INFO_KEYS = [
    "fc_reward",
    "efficiency_reward",
    "step_soc_reward",
    "action.engine_stop",
    "action.power_request",
]
LOGGED_INFO_KEYS = (
    [f"{key}_sum" for key in INFO_KEYS]
    + [f"{key}_avg" for key in INFO_KEYS]
    + ["end_soc_reward"]
)

ENV_CONFIG = {
    "config_path": "reev_control/envs/config.yaml",
    "obs_seq_len": 600,
    "data_start_index": 600,
    "data_min_length": 3600,
    "step_size_in_seconds": 10,
    "reward_weights": [1, 1, 0.1, 0.05],
    "file_list_file": "data/train/Mar2025_filtered_files.pkl",
}

TRAIN_CONFIG = {
    "n_envs": 8,
    "policy_type": "MlpPolicy",
    "total_timesteps": 5_000_000,
    "n_steps": 512,
    "batch_size": 256,
    "n_epochs": 10,
    "gamma": 0.98,
    # "gamma": 0.99,
    "gae_lambda": 0.98,
    # "gae_lambda": 0.95,
    "learning_rate": 3e-4,
    "ent_coef": 0.05,
    "vf_coef": 0.25,
    # "vf_coef": 0.15,
    "device": "cpu",
    "vecnorm_gamma": 0.95,
    "seed": 100,
    "normalize_reward": False
}


# ==============================================================================
# Environment Factory
# ==============================================================================

def make_env(seed: int | None = None, **kwargs):
    """Create a wrapped environment instance."""
    def _init():
        env = SimpleVehicleEnv4(data_folder="data/train/REEV07RearDrive_Mar2025", seed=seed, **kwargs)
        env = InfoHistoryWrapper(env, info_keys=INFO_KEYS)
        env = Monitor(env, info_keywords=LOGGED_INFO_KEYS)
        env.reset()
        return env
    return _init


# ==============================================================================
# Training
# ==============================================================================

def parse_config_value(value: str):
    """Parse value as int, float, bool, or keep as string."""
    try:
        return int(value)
    except ValueError:
        try:
            return float(value)
        except ValueError:
            if value.lower() == "true":
                return True
            elif value.lower() == "false":
                return False
            return value


def     apply_cli_config(args: argparse.Namespace) -> None:
    """Apply CLI args to ENV_CONFIG and TRAIN_CONFIG."""
    for key in vars(args):
        if key in ("run", "notes"):
            continue
        val = getattr(args, key)
        if val is not None:
            parsed_val = parse_config_value(val)
            if key in TRAIN_CONFIG:
                TRAIN_CONFIG[key] = parsed_val
            elif key in ENV_CONFIG:
                ENV_CONFIG[key] = parsed_val
            else:
                raise ValueError(f"Unknown config key: {key}")


def train(args: argparse.Namespace) -> None:
    """Main training loop."""
    global ENV_CONFIG, TRAIN_CONFIG

    apply_cli_config(args)

    os.environ["WANDB_DIR"] = "train_results"

    date_str = datetime.datetime.now().strftime("%Y%m%d")
    run_name = args.run or f"PPO_env4_{date_str}"
    run_id = f"{date_str}_{uuid.uuid4().hex[:8]}"

    run = wandb.init(
        project="reev_control",
        id=run_id,
        name=run_name,
        config={"TRAIN_CONFIG": TRAIN_CONFIG, "ENV_CONFIG": ENV_CONFIG},
        sync_tensorboard=True,
        monitor_gym=True,
        save_code=True,
        notes=args.notes,
    )


    # Create vectorized environment
    vec_env = SubprocVecEnv([
        make_env(seed=TRAIN_CONFIG['seed'] + i, **ENV_CONFIG)
        for i in range(TRAIN_CONFIG["n_envs"])
    ])

    # Normalize observations
    if "vecnorm_load_path" in TRAIN_CONFIG:
        vec_env = VecNormalize.load(TRAIN_CONFIG["vecnorm_load_path"], vec_env)
    else:
        vec_env = VecNormalize(
            vec_env,
            training=True,
            norm_obs=True,
            norm_reward=TRAIN_CONFIG['normalize_reward'],
        )

    # Create or load model
    if "model_load_path" in TRAIN_CONFIG:
        model = CustomPPO.load(
            TRAIN_CONFIG["model_load_path"],
            env=vec_env,
            device=TRAIN_CONFIG["device"],
        )
    else:
        model = CustomPPO(
            policy=TRAIN_CONFIG["policy_type"],
            env=vec_env,
            verbose=1,
            device=TRAIN_CONFIG["device"],
            n_steps=TRAIN_CONFIG["n_steps"],
            batch_size=TRAIN_CONFIG['batch_size'],
            n_epochs=TRAIN_CONFIG['n_epochs'],
            gamma=TRAIN_CONFIG["gamma"],
            gae_lambda=TRAIN_CONFIG["gae_lambda"],
            learning_rate=linear_schedule(TRAIN_CONFIG["learning_rate"]),
            ent_coef=TRAIN_CONFIG["ent_coef"],
            vf_coef=TRAIN_CONFIG["vf_coef"],
            tensorboard_log=f"train_results/tensorboard/{run.id}",
            info_keys=LOGGED_INFO_KEYS,
            use_sde=True,
            policy_kwargs=dict(squash_output=True),
        )

    callbacks = CallbackList([
        WandbCallbackWithVecNorm(
            gradient_save_freq=100,
            model_save_path=f"train_results/models/{run.id}",
            model_save_freq=10_000,
            verbose=2,
        ),
        CheckpointCallback(
            save_freq=10_000,
            save_path=f"train_results/models/{run.id}/checkpoints/",
            save_vecnormalize=True,
        ),
    ])

    model.learn(
        total_timesteps=TRAIN_CONFIG["total_timesteps"],
        callback=callbacks,
        log_interval=1,
    )

    run.finish()


# ==============================================================================
# Entry Point
# ==============================================================================

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Train PPO model for REEV control")
    parser.add_argument("--run", type=str, help="Run name for wandb")
    parser.add_argument("--notes", type=str, help="Notes for wandb run")
    parser.add_argument("--device", type=str, default=None, help="Device (e.g., 'cpu', 'cuda:0')")

    # Add config keys as CLI arguments
    for key in ENV_CONFIG:
        if key not in ("config_path", "file_list_file"):
            parser.add_argument(f"--{key}", type=str, default=None)
    for key in TRAIN_CONFIG:
        if key not in ("device",):
            parser.add_argument(f"--{key}", type=str, default=None)

    args = parser.parse_args()
    train(args)
