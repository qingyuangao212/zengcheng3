import os
import pandas as pd
import gymnasium as gym
import numpy as np
from gymnasium import spaces

class TrajectoryLoader:
    def __init__(self, data_folder, step_size=1, min_length=1800, file_list_file=None, seed=None):
        """
        A trajectory loader that iterates over CSV files in a folder, reading them lazily.

        Args:
        - data_folder (str): Path to the folder containing trajectory CSV files.
        - step_size (int): The number of seconds
        - min_length (int): Minimum length of trajectory to be considered valid.
        - file_list_file (list): Optional pickle file to load a list of file names.
        """

        self.data_folder = data_folder
        self.step_size = step_size
        self.min_length = min_length

        if file_list_file:
            import pickle
            with open('file_list_file', 'rb') as f:
                self.file_list = pickle.load(f)
        else:
            self.file_list = [os.path.join(self.data_folder, f) for f in os.listdir(self.data_folder) if f.endswith(".csv")]
            
        self.np_random = np.random.default_rng(seed) # set self.np_random
        self.reset()
        self.num_iterations = 0  # Track number of times all files have been iterated

        self.trajectory_counter  = 0    # num of trajectories loaded so far
        
        
    def reset(self):

        self.file_idx = 0
        self.np_random.shuffle(self.file_list)


    def load_trajectory(self):
        """Load the next CSV file and track iterations over the dataset.
           TBD: aggregate features by step_size, mostly by just taking last value
        """
        if not self.file_list:
            raise FileNotFoundError(f"No CSV files found in {self.data_folder}")
        
        while True:
            if self.file_idx >= len(self.file_list):
                self.num_iterations += 1
                self.reset()
                
            file_path = self.file_list[self.file_idx]
            self.file_idx += 1

            data = pd.read_csv(file_path)

            if len(data) < self.min_length:
                print(f"[TrajectoryLoader] Skipping {file_path} (len={len(data)} < min_len={self.min_length})")
                continue

            else:
                data = self.preprocess(data)
                self.trajectory_counter += 1
                return data
            
            
    def preprocess(self, data):
        # TBD
        # use self.step_size to aggregate data
        
        data = self.ad_hoc_processing(data)
        data = add_fake_navigation(data)
        return data

    def ad_hoc_processing(self, data):
        data = data.ffill()
        data = data.bfill()
        data.fillna(0, inplace=True)

        return data
    
    def get_iterations(self):
        """Return the number of full iterations over the dataset."""
        return self.num_iterations

    def set_np_random(self, np_random: np.random._generator.Generator):
        self.np_random = np_random  # numpy.random._generator.Generator

def add_fake_navigation(data: pd.DataFrame, n_quantiles=10) -> pd.DataFrame:
    """
    Adds fake navigation-related features to the given DataFrame.

    The function computes the following new columns:
    - mileage_from_start: Distance from the starting mileage.
    - time_from_start: Time steps from the start (index-based).
    - nav_mileage_togo: Estimated remaining mileage to go.
    - nav_time_togo: Estimated remaining time steps to go.
    - EspVehSpd_qX: Quantile values (X in [0, 100]) of 'EspVehSpd' from the current row to the end.

    Args:
        data (pd.DataFrame): Input DataFrame containing at least the columns 'CdcTotMilg' and 'EspVehSpd'.
        n_quantiles (int): Number of quantiles to compute (default is 10, producing 11 quantile points from 0% to 100%).

    Returns:
        pd.DataFrame: The original DataFrame with additional navigation and quantile-related columns.
    """
    data['mileage_from_start'] = data['CdcTotMilg'] - data['CdcTotMilg'].iloc[0]
    data['time_from_start'] = np.arange(len(data))
    data['nav_mileage_togo'] = data['CdcTotMilg'].iloc[-1] - data['CdcTotMilg']
    data['nav_time_togo'] = len(data) - data['time_from_start'] # in seconds
    
    # Define quantile fractions (0.0 to 1.0)
    quantile_fractions = [i / n_quantiles for i in range(n_quantiles + 1)]
    nav_speed_quantiles = [f"nav_speed_q{int(q * 100)}" for q in quantile_fractions]
    
    # Initialize list to store quantile values
    quantiles_list = []

    # Loop through each row and compute quantiles from that row to the end
    for idx in range(len(data)):
        spd_slice = data.loc[idx:, "EspVehSpd"]
        quantiles = spd_slice.quantile(q=quantile_fractions, interpolation="linear").values
        quantiles_list.append(quantiles)

    # Add new quantile columns
    for i, col in enumerate(nav_speed_quantiles):
        data[col] = [q[i] for q in quantiles_list]

    return data

@property
def file_name(self):
    return self.file_list[self.file_index]

