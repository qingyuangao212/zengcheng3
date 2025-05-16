import torch
import torch.nn as nn
import gymnasium as gym
from stable_baselines3.common.torch_layers import BaseFeaturesExtractor
from stable_baselines3 import PPO
from stable_baselines3.common.env_util import make_vec_env

class CustomFeatureExtractor(BaseFeaturesExtractor):
    def __init__(self, observation_space: gym.spaces.Dict, features_dim=256):
        super().__init__(observation_space, features_dim)

        # Assume observation_space contains:
        # 1. "time_series": (sequence_length, num_features)
        # 2. "stats": (num_stats,)

        time_series_shape = observation_space["sequential"].shape
        num_stats = observation_space["non-sequential"].shape[0]

        # CNN for time-series input (alternatively, use an RNN)
        self.time_series_net = nn.Sequential(
            nn.Conv1d(in_channels=time_series_shape[1], out_channels=16, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            nn.Flatten(),
            nn.Linear(time_series_shape[0] * 16, 128),
            nn.ReLU()
        )

        # Fully connected network for statistics
        self.stats_net = nn.Sequential(
            nn.Linear(num_stats, 64),
            nn.ReLU()
        )

        # Final aggregation layer
        self.fc = nn.Sequential(
            nn.Linear(128 + 64, features_dim),
            nn.ReLU()
        )

    def forward(self, observations):
        # Process time series input
        time_series = observations["time_series"].permute(0, 2, 1)  # Reshape for Conv1D (batch, channels, seq)
        time_series_features = self.time_series_net(time_series)

        # Process statistics input
        stats_features = self.stats_net(observations["stats"])

        # Concatenate and pass through final layers
        combined = torch.cat([time_series_features, stats_features], dim=1)
        return self.fc(combined)
