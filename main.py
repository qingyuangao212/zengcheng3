#!/usr/bin/env python
"""Main entry point for REEV control project."""

import argparse
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description="REEV Control - Reinforcement Learning")
    parser.add_argument(
        "command",
        choices=["train", "eval", "inference"],
        help="Command to run",
    )
    parser.add_argument("--run", type=str, help="Run name for training")
    parser.add_argument("--device", type=str, default="cuda", help="Device to use (e.g., cuda, cuda:0, cpu)")
    parser.add_argument("--model-path", type=str, help="Path to model for evaluation/inference")
    parser.add_argument("--n-episodes", type=int, default=10, help="Number of episodes for evaluation")

    args = parser.parse_args()

    if args.command == "train":
        if not args.run:
            print("Error: --run is required for training")
            sys.exit(1)
        cmd = ["python", "scripts/sb3_ppo_train_env4.py", "--run", args.run, "--device", args.device]
        subprocess.run(cmd)

    elif args.command == "eval":
        if not args.model_path:
            print("Error: --model-path is required for evaluation")
            sys.exit(1)
        cmd = ["python", "scripts/evaluate_v3.py", "--model-path", args.model_path, "--n-episodes", str(args.n_episodes)]
        subprocess.run(cmd)

    elif args.command == "inference":
        cmd = ["python", "scripts/inference_fe.py"]
        subprocess.run(cmd)


if __name__ == "__main__":
    main()
