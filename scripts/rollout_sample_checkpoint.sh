#!/bin/bash

# Usage: ./rollout_sample_checkpoint.sh <run_name> <checkpoint>
# Example: ./rollout_sample_checkpoint.sh 20260312_ed72d449 720000



RUN_NAME="20260312_ed72d449"
CHECKPOINT="720000"

# Define paths
BASE_DIR="train_results/models"
MODEL_PATH="${BASE_DIR}/${RUN_NAME}/checkpoints/rl_model_${CHECKPOINT}_steps.zip"
VECNORM_PATH="${BASE_DIR}/${RUN_NAME}/checkpoints/rl_model_vecnormalize_${CHECKPOINT}_steps.pkl"
OUTPUT_DIR="${BASE_DIR}/${RUN_NAME}/rollout_samples_${CHECKPOINT}"

# Run rollout_sample.py
cd /home/avatr/qingyuan/zengcheng3
uv run scripts/rollout_sample.py \
    --run "$RUN_NAME" \
    --checkpoint "$CHECKPOINT" \
    --model_path "$MODEL_PATH" \
    --vecnorm_path "$VECNORM_PATH" \
    --output_dir "$OUTPUT_DIR" \
    --num_episodes 10
