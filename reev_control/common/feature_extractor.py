import torch
import torch.nn as nn
import gymnasium as gym
from stable_baselines3.common.torch_layers import BaseFeaturesExtractor
from stable_baselines3 import PPO
from stable_baselines3.common.env_util import make_vec_env

class LSTMFeatureExtractor(BaseFeaturesExtractor):
    def __init__(self, observation_space: gym.spaces.Dict, features_dim=256):
        super().__init__(observation_space, features_dim)

        # Extract shapes
        time_series_shape = observation_space["sequential"].shape  # (seq_len, num_features)
        num_stats = observation_space["non-sequential"].shape[0]

        seq_len, num_features = time_series_shape

        self.lstm_hidden_dim = 64
        self.lstm_layers = 1

        # LSTM for time-series
        self.lstm = nn.LSTM(
            input_size=num_features,
            hidden_size=self.lstm_hidden_dim,
            num_layers=self.lstm_layers,
            batch_first=True
        )

        # Stats MLP
        self.stats_net = nn.Sequential(
            nn.Linear(num_stats, 64),
            nn.ReLU()
        )

        # Final aggregation MLP
        self.fc = nn.Sequential(
            nn.Linear(self.lstm_hidden_dim + 64, features_dim),
            nn.ReLU()
        )

    def forward(self, observations):
        # LSTM expects shape: (batch, seq_len, features)
        time_series = observations["sequential"]  # (batch, seq_len, features)
        batch_size = time_series.size(0)

        # Initialize LSTM hidden state (optional: put on correct device)
        h0 = torch.zeros(self.lstm_layers, batch_size, self.lstm_hidden_dim, device=time_series.device)
        c0 = torch.zeros(self.lstm_layers, batch_size, self.lstm_hidden_dim, device=time_series.device)

        # LSTM forward
        lstm_out, (hn, cn) = self.lstm(time_series, (h0, c0))
        # Use last hidden state as feature
        time_series_features = hn[-1]  # (batch, hidden_dim)

        # Stats branch
        stats_features = self.stats_net(observations["non-sequential"])

        # Concatenate both
        combined = torch.cat([time_series_features, stats_features], dim=1)
        return self.fc(combined)
