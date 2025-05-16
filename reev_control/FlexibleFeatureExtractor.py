import torch
import torch.nn as nn
import torch.nn.functional as F
from stable_baselines3.common.torch_layers import BaseFeaturesExtractor
from gym import spaces
import numpy as np

class FlexibleFeatureExtractor(BaseFeaturesExtractor):
    def __init__(self, observation_space: spaces.Dict, seq_processing: str = "lstm"):
        super().__init__(observation_space, features_dim=1)  # Placeholder
        
        self.seq_processing = seq_processing.lower()
        sequential_shape = observation_space["sequential"].shape  # (obs_seq_len, num_seq_features)
        non_sequential_shape = observation_space["non-sequential"].shape[0]
        self.seq_len, self.seq_features = sequential_shape
        
        # Normalization layers
        self.seq_norm = nn.LayerNorm(self.seq_features)
        self.non_seq_norm = nn.LayerNorm(non_sequential_shape)
        
        if self.seq_processing == "lstm":
            seq_output_dim = 64
            self.seq_model = nn.LSTM(input_size=self.seq_features, hidden_size=seq_output_dim, batch_first=True)
            
        elif self.seq_processing == "cnn":
            self.seq_model = nn.Sequential(
                nn.Conv1d(in_channels=self.seq_features, out_channels=32, kernel_size=3, padding=1),
                nn.ReLU(),
                nn.Conv1d(in_channels=32, out_channels=64, kernel_size=3, padding=1),
                nn.ReLU(),
                nn.AdaptiveAvgPool1d(1)  # Reduce over the sequence dimension
            )
            seq_output_dim = 64  # Output is (batch, channels, 1), so we squeeze it
        elif self.seq_processing in ["ewma", "mean"]:
            seq_output_dim = self.seq_features
        else:
            raise ValueError(f"Unknown sequential processing type: {self.seq_processing}")
        
        self.features_dim = seq_output_dim + non_sequential_shape

    def forward(self, observations):
        seq_data = observations["sequential"]  # (batch, seq_len, seq_features)
        non_seq_data = observations["non-sequential"]  # (batch, non_seq_features)
        
        # Normalize inputs
        seq_data = self.seq_norm(seq_data)
        non_seq_data = self.non_seq_norm(non_seq_data)
        
        if self.seq_processing == "lstm":
            _, (h_n, _) = self.seq_model(seq_data)  # h_n is (1, batch, hidden_size)
            seq_features = h_n.squeeze(0)  # (batch, hidden_size)
        elif self.seq_processing == "cnn":
            seq_data = seq_data.permute(0, 2, 1)  # Change to (batch, features, seq_len)
            seq_features = self.seq_model(seq_data).squeeze(-1)  # (batch, 64)
        elif self.seq_processing == "ewma":
            alpha = 0.2
            weights = torch.exp(torch.arange(self.seq_len - 1, -1, -1, dtype=torch.float32, device=seq_data.device) * -alpha)
            weights = weights / weights.sum()
            seq_features = (seq_data * weights.view(1, -1, 1)).sum(dim=1)
        elif self.seq_processing == "mean":
            seq_features = seq_data.mean(dim=1)
        
        return torch.cat([seq_features, non_seq_data], dim=-1)
