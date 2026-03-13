import os
import argparse
import random
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

from stable_baselines3.common.vec_env import DummyVecEnv, VecNormalize
from reev_control.custom_ppo import CustomPPO
from sb3_ppo_train_env4 import make_env, ENV_CONFIG


def plot_episode(df: pd.DataFrame, save_path: str, episode_idx: int):
    """
    ax1 (left): end_speed + SOC
    ax2 (right): drive_power (kW) + power_request
    Highlight region where action.engine_stop == True
    """
    # Set seaborn theme
    sns.set_theme(style="whitegrid")
    palette = sns.color_palette("muted")

    fig, ax1 = plt.subplots(figsize=(12, 6))
    t = np.arange(len(df))

    # -------------------------
    # Highlight engine stop regions (very light shade)
    # -------------------------
    engine_stop = df["action.engine_stop"].astype(bool).values

    in_region = False
    start_idx = 0

    for i, val in enumerate(engine_stop):
        if val and not in_region:
            in_region = True
            start_idx = i
        elif not val and in_region:
            ax1.axvspan(start_idx, i, color="orange", alpha=0.1)
            in_region = False

    # If episode ends during engine stop
    if in_region:
        ax1.axvspan(start_idx, len(engine_stop), color="orange", alpha=0.1)

    # -------------------------
    # Left axis (Speed + SOC)
    # -------------------------
    ax1.plot(t, df["end_speed"],
             color="#6baed6", alpha=0.6, linewidth=2,
             label="Speed")

    ax1.plot(t, df["BcuEnyMagtSoc"],
             color="#2ca02c", alpha=0.8, linewidth=3,
             label="SOC")

    ax1.set_xlabel("Time Step")
    ax1.set_ylim(0, 120)
    ax1.legend(loc="upper left")

    # -------------------------
    # Right axis (Powers)
    # -------------------------
    ax2 = ax1.twinx()

    # Highlight power_request NaN regions (engine stopped)
    power_request = df["action.power_request"]
    nan_mask = power_request.isna().values
    in_nan = False
    start_idx = 0
    for i, val in enumerate(nan_mask):
        if val and not in_nan:
            in_nan = True
            start_idx = i
        elif not val and in_nan:
            ax2.axvspan(start_idx, i, color="orange", alpha=0.2)
            in_nan = False
    if in_nan:
        ax2.axvspan(start_idx, len(nan_mask), color="orange", alpha=0.2)

    # Drive Power - gray, thinner line
    drive_power_kw = df["drive_power"] / 1000.0
    ax2.plot(t, drive_power_kw,
             color="gray", alpha=0.5, linewidth=1.5,
             linestyle="--",
             label="Drive Power (kW)")

    # Power Request - only plot non-NaN values
    valid_mask = ~power_request.isna()
    ax2.plot(t[valid_mask], power_request[valid_mask],
             color="#d62728", alpha=0.8, linewidth=3,
             linestyle="-.",
             label="Power Request")

    # Fixed y-axis for powers
    ax2.set_ylim(-150, 150)
    ax2.legend(loc="upper right")

    # -------------------------
    # Final touches
    # -------------------------
    plt.title(f"Episode {episode_idx}")
    plt.tight_layout()
    plt.savefig(save_path)
    plt.close()


def main(args):

    # ----------------------------
    # Resolve paths (with defaults)
    # ----------------------------
    run_name = args.run
    num_episodes = args.num_episodes

    model_path = (
        args.model_path
        if args.model_path is not None
        else f"train_results/models/{run_name}/model.zip"
    )

    vecnorm_path = (
        args.vecnorm_path
        if args.vecnorm_path is not None
        else f"train_results/models/{run_name}/vec_env.pkl"
    )

    output_dir = (
        args.output_dir
        if args.output_dir is not None
        else f"train_results/models/{run_name}/rollout_samples"
    )

    os.makedirs(output_dir, exist_ok=True)

    excel_output = os.path.join(
        output_dir, f"{run_name}_inference_{num_episodes}eps.xlsx"
    )

    print("🚀 Starting inference...")
    print(f"Model path: {model_path}")
    print(f"VecNorm path: {vecnorm_path}")
    print(f"Output dir: {output_dir}")

    # ----------------------------
    # Create environment
    # ----------------------------
    # ENV_CONFIG.update({"step_size_in_seconds": 5})
    seed = args.seed if args.seed is not None else random.randint(0, 100_000)
    dummy_vecenv = DummyVecEnv([make_env(seed=seed, **ENV_CONFIG)])
    dummy_vecenv = VecNormalize.load(vecnorm_path, dummy_vecenv)

    dummy_vecenv.training = False

    # ----------------------------
    # Load model
    # ----------------------------
    model = CustomPPO.load(model_path, device="cpu")

    print("📥 Starting multi-episode inference...")

    with pd.ExcelWriter(excel_output, engine="xlsxwriter") as writer:

        for ep in range(num_episodes):
            print(f"\n🚀 Episode {ep + 1} starting...")

            obs = dummy_vecenv.reset()
            done = False
            episode_infos = []

            while not done:
                action, _ = model.predict(obs, deterministic=True)
                obs, rewards, done, info = dummy_vecenv.step(action)
                episode_infos.append(info[0])

            print(f"✅ Episode {ep + 1} done. Collected {len(episode_infos)} steps.")

            df = pd.DataFrame(episode_infos)

            df.to_excel(writer, sheet_name=f"episode_{ep+1}", index=False)

            plot_path = os.path.join(output_dir, f"{run_name}_episode_{ep+1}.png")
            plot_episode(df, plot_path, ep + 1)

    print(f"\n💾 Saved Excel to {excel_output}")
    print(f"📊 Plots saved to {output_dir}")


if __name__ == "__main__":

    parser = argparse.ArgumentParser()

    parser.add_argument("--run", type=str, default="1cnx6npg")
    parser.add_argument("--seed", type=int, default=None)
    parser.add_argument("--num_episodes", type=int, default=10)

    parser.add_argument("--model_path", type=str, default=None)
    parser.add_argument("--vecnorm_path", type=str, default=None)
    parser.add_argument("--output_dir", type=str, default=None)
    parser.add_argument("--checkpoint", type=int)
    args = parser.parse_args()

    main(args)