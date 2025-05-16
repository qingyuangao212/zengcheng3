# import os
# os.environ['MKL_THREADING_LAYER'] = 'GNU'
import gymnasium as gym
from stable_baselines3.common.vec_env import SubprocVecEnv

# Your custom environment
from reev_control.envs.SimpleVehicleEnv import SimpleVehicleEnv

# Function to create environment instances for each subprocess
def make_env():
    return SimpleVehicleEnv(data_folder='data/train/REEV07RearDrive_Jan2025')

if __name__ == "__main__":
    
    # Create the vectorized environment using SubprocVecEnv (parallel environments)
    env = SubprocVecEnv([make_env for _ in range(4)]) 
    print(env)