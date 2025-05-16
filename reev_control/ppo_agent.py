import torch
import gymnasium as gym
import numpy as np
from stable_baselines3 import PPO
from stable_baselines3.common.torch_layers import BaseFeaturesExtractor
from stable_baselines3.common.envs import DummyVecEnv
from gymnasium import spaces
from CustomFeatureExtractor import CustomFeatureExtractor  # Import your extractor
from reev_control.envs.VehicleEnv import VehicleTrajectoryEnv  # Import your environment

# Create vectorized environment
env = DummyVecEnv([lambda: VehicleTrajectoryEnv()])

# Define policy with custom feature extractor
policy_kwargs = {
    "features_extractor_class": CustomFeatureExtractor,
    "features_extractor_kwargs": {"seq_processing": "lstm"},  # Options: "lstm", "cnn", "ewma", "mean"
    "net_arch": [64, 64],  # Two hidden layers of 64 neurons each for policy and value networks
}

model = PPO(
    "MultiInputPolicy",
    env,
    policy_kwargs=policy_kwargs,  # Custom feature extractor
    learning_rate=2.5e-4,         # Reduce learning rate for stability
    n_steps=1024,                 # Reduce rollout buffer size for faster updates
    batch_size=128,               # Use larger batch size for stability
    n_epochs=20,                  # More epochs for better convergence
    gamma=0.98,                   # Slightly higher discount factor
    gae_lambda=0.92,              # Adjust GAE smoothing factor
    clip_range=0.15,              # Reduce clipping range for stability
    ent_coef=0.01,                # Encourage more exploration
    vf_coef=0.4,                  # Slightly lower value loss coefficient
    max_grad_norm=0.5,            # Keep default gradient clipping
    verbose=1
)

model.learn(total_timesteps=200000)  # Train for longer

