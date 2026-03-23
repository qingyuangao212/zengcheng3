"""2025/05/22A version with simplified action space: power request only
Instead of searching over feasible rspd and tq for best efficiency, use a predefined function to compute rspd and tq requests

2025/06/09 add start/stop action, add file_list_file integration

2025/06/18 add feature extractor: envrionment observation space is a Dict with full sequential data

2026/01/04: add note, no controller used in v3

2026/01/20: new version v4

2026/01/23： discovered bug in simulator.reset(), causes soc and all outputs to be 0
change: make initial_soc random draw to be a class attribute and pass to step method 

2026/02/13: write new env4 (without FE) with the following changes:
1. action space is now Box(0, 100), where <3 means power off

"""

import yaml
import numpy as np
import pandas as pd
import gymnasium as gym
from gymnasium import spaces
# It's important to write out the module despite the evns/__init__.py.  Avoid circular imports
from reev_control.envs.trajectory_loader import TrajectoryLoader
# from reev_control.envs.BaseController import BaseControllerV2
from reev_control.envs.simulator import Simulator
from reev_control.envs.reward import step_reward

from reev_control.envs.utils import compute_drive_power, spd_power_to_tq_rspd


class SimpleVehicleEnv4(gym.Env):
   

    def __init__(self,
                 data_folder='data/train/REEV07RearDrive_Mar2025',
                 config_path="reev_control/envs/config.yaml",
                 **kwargs):
        """
        Initializes the VehicleTrajectoryEnv.

        Args:
            data_folder (str): Path to the folder containing trajectory CSV files.
            simulator: Object handling system dynamics and low-level control.
            reward_fn (function): Function to compute reward based on state, action, and simulator outputs.
            config_path (str): Path to the environment configuration YAML file.
        """
        super().__init__()

        with open(config_path, "r") as f:
            self.config = yaml.safe_load(f)
        self.config.update(kwargs)  # update config with kwargs
        self.seed = self.config.get(
            'seed'
        )  # Note bug：if seed is passed then random number in reset (soc) will be the same every time

        self.step_size_in_seconds = self.config.get('step_size_in_seconds', 10)
        self.step_size_in_10ms = 100 * self.step_size_in_seconds

        self.obs_seq_len = self.config.get("obs_seq_len", 30)

        self.trajectory_loader = TrajectoryLoader(
            data_folder=data_folder,
            step_size=self.step_size_in_seconds,
            min_length=self.config.get('data_min_length', 1800),
            file_list_file=self.config.get('file_list_file', None),  # optional file list
            seed=self.seed  # manages shuffling of data files; if not passed just random shuffle
        )

        # self.base_controller = None # deprecated

        self.simulator = Simulator(self.config['simulator_model_path'])

        self.reward_fn = lambda *args: step_reward(
            *args, self.config.get("reward_weights", [1, 1, 1, 1]
                                   ))  # allow setting reward weights

        # Define observation space
        self.observation_space = spaces.Box(
            low=-np.inf,
            high=np.inf,
            shape=(len(self.config["state_variables"]["sequential"]) * 5 +
                   len(self.config["state_variables"]["non-sequential"]) +
                   len(self.config['simulator_state_vars']) + 4, ))

        # Define action space
        self.action_space = spaces.Box(low=0, high=100, dtype=np.float32)  

    def reset(self, seed=None, options=None):
        """
        Resets the environment at the beginning of an episode.

        Loads the next trajectory file, resets the step index, initializes 
        state variables, and computes the initial observation.

        Args:
            seed (int, optional): Random seed for reproducibility.
            options (dict, optional): Additional options for reset.

        Returns:
            tuple: (initial observation, empty info dictionary).

        """
        if seed is not None:
            self.seed = seed
        super().reset(seed=self.seed, options=options)  # np_random
        self.trajectory = self.trajectory_loader.load_trajectory(
        )  # need to aggregate data by step size
        self.step_idx = self.config['data_start_index']

        # reinit vehicle simulator with random start_soc
        self.initial_soc = self.np_random.uniform(40, 60)

        self.simulator.reset({"BcuEnyMagtSoc_Inital": self.initial_soc})
        # self.simulator = Simulator(self.config['simulator_model_path'])

        # Initialize self.state to all zeros except SOC
        initial_simulated_state = dict.fromkeys(
            self.config['simulator_state_vars'], 0)
        initial_simulated_state['BcuEnyMagtSoc'] = self.initial_soc

        self.state = self._compute_observation(initial_simulated_state)

        return self.state, {"BcuEnyMagtSoc": self.initial_soc}

    def step(self, action):
      
        action = action[0]  # convert single-element array to float
        info = {}

        assert (self.step_idx < len(self.trajectory) - 1)
        assert (self.step_idx + self.step_size_in_seconds
                < len(self.trajectory))

        # 1.a compute speed every 10ms within the step: use the next step speed and assume constant acceleration
        speed_seq, drive_power_seq = self._compute_speed_and_power_seq()

        # parse action
        engine_stop = action <=3.5  # threshold for engine stop, can tune later

        if engine_stop:

            power_request_seq = np.zeros_like(speed_seq)
            torque_request_seq, rspd_request_seq = np.zeros_like(
                speed_seq), np.zeros_like(speed_seq)

        else:  
            power_request_seq = np.tile(action, len(speed_seq))  # constant
            torque_request_seq, rspd_request_seq = spd_power_to_tq_rspd(
                speed_seq, power_request_seq)

        info.update({
            "start_speed": speed_seq[0],
            "end_speed": speed_seq[-1],
            "drive_power": drive_power_seq[-1],
            "action.engine_stop": engine_stop,
            "action.power_request": np.nan if engine_stop else action,
            "torque_request": torque_request_seq[-1],
            "rspd_request": rspd_request_seq[-1]
        })

        assert (len(speed_seq) == len(drive_power_seq) ==
                len(torque_request_seq) == len(rspd_request_seq) ==
                self.step_size_in_10ms)

        # ================= 3. SIMULATOR =================
        simulator_outputs_df = self._run_simulator(torque_request_seq,
                                                   rspd_request_seq)

        # ================= 4. RL done, reward, state_prime =================
        # compute episode done flag: determine done if the next step cannot compute a s_prime
        # meaning that two step sizes away, index is greater than the last index
        done = (self.step_idx + 2 * self.step_size_in_seconds
                > len(self.trajectory) - 1)

        reward_inputs = simulator_outputs_df[
            self.config['simulator_reward_vars']].to_dict(orient='list')
        reward_inputs['speed_seq'] = speed_seq

        info.update(simulator_outputs_df.mean().to_dict())

        # Override EmsFuCns with sum instead of mean
        info['EmsFuCns'] = simulator_outputs_df['EmsFuCns'].sum()

        # compute step reward
        step_reward, reward_info = self.reward_fn(reward_inputs, done)

        info['done'] = done
        info.update(reward_info)

        self.step_idx += self.step_size_in_seconds  # update step_idx must preceed compute_observation
        # compute s_prime
        simulated_states = simulator_outputs_df.iloc[-1].loc[
            self.config['simulator_state_vars']].to_dict()
        self.state = self._compute_observation(simulated_states)

        truncated = False  # No truncation for now, for the general case where done can be set:  truncated = done and (self.step_idx + 1 < len(self.trajectory) - 1)

        return self.state, step_reward, done, truncated, info

    def _get_sequential_data(self):
        """sequential data are aggregated by minute, and padded to fixed length"""
        cols = self.config["state_variables"]["sequential"]
        start_idx = max(0, self.step_idx - self.obs_seq_len + 1)

        # draw sequential data and non-sequential data from trajectory, using step_index
        sequential_data = self.trajectory.iloc[start_idx:self.step_idx +
                                               1][cols]

        out = np.concatenate([
            sequential_data.mean(),
            sequential_data.std(), 
            sequential_data.max(), 
            sequential_data.median(),
            sequential_data.iloc[-1]
        ])

        return out

    def _get_non_sequential_data(self):

        non_sequential_data = self.trajectory.iloc[self.step_idx][
            self.config["state_variables"]["non-sequential"]].values

        return non_sequential_data

    def _get_step_size_window_data(self):
        """Get future mean and std of EspVehSpd and EspLgtAccel over the next `step_size_in_seconds` window."""
        future_cols = ['EspVehSpd', 'EspLgtAccel']
        # Future window: next step_size_in_seconds rows (trajectory is 1-second resolution)
        future_end_idx = min(self.step_idx + self.step_size_in_seconds + 1, len(self.trajectory))
        future_data = self.trajectory.iloc[self.step_idx + 1 : future_end_idx][future_cols]

        # Pad if trajectory near end
        if len(future_data) < self.step_size_in_seconds:
            pad_len = self.step_size_in_seconds - len(future_data)
            if len(future_data) > 0:
                last_vals = future_data.iloc[-1]
            else:
                last_vals = self.trajectory.iloc[self.step_idx][future_cols]
            pad_df = pd.DataFrame([last_vals] * pad_len, columns=future_cols)
            future_data = pd.concat([future_data, pad_df], ignore_index=True)

        return np.array([
            future_data['EspVehSpd'].mean(),
            future_data['EspVehSpd'].std(),
            future_data['EspLgtAccel'].mean(),
            future_data['EspLgtAccel'].std(),
        ])

    def _compute_observation(self, simulated_states: dict):
        """
        Computes the current observation.

        The observation includes:
        - Sequential data: Past `obs_seq_len` steps of selected state variables.
        - State data: Current step's state variables plus additional computed values.

        Returns:
            dict: Dictionary containing 'sequential' and 'state' keys.
        """

        sequential_data = self._get_sequential_data()

        non_sequential_data = self._get_non_sequential_data()

        step_size_window_data = self._get_step_size_window_data()

        # Add simulator values (always update tq, n, soc before updating state)
        obs = np.append(np.concatenate([sequential_data, non_sequential_data, step_size_window_data]),
                        list(simulated_states.values())).astype(np.float32)

        return obs

    # def _compute_speed_and_power_seq(self):

    #     start_speed = self.trajectory['EspVehSpd'].iloc[self.step_idx]
    #     end_speed = self.trajectory['EspVehSpd'].iloc[self.step_idx + 1]
    #     speed_seq = np.linspace(start_speed, end_speed,
    #                             self.step_size_in_10ms + 1)[:-1]

    #     # 1.b compute drive_power every 10ms （发动机外特性查表）
    #     constant_accel = (end_speed - start_speed) / self.step_size_in_seconds
    #     drive_power_seq = compute_drive_power(speed_seq, constant_accel)
    #     return speed_seq, drive_power_seq

    def _compute_speed_and_power_seq(self):
        """Note: trajectory data is in seconds, but need to output sequence in 10ms frequency. Upsample 100 times"""
        upsample = 100
        speed_points = self.trajectory['EspVehSpd'].iloc[
            self.step_idx:self.step_idx + self.step_size_in_seconds +
            1].to_numpy()  # speed at every second within the step_size

        speed_seq = np.concatenate([
            np.linspace(speed_points[i],
                        speed_points[i + 1],
                        upsample,
                        endpoint=False) for i in range(len(speed_points) - 1)
        ])  # speed array every 10ms

        acc_seq = np.repeat((speed_points[1:] - speed_points[:-1]),
                            upsample)  # acceleration

        drive_power_seq = compute_drive_power(speed_seq, acc_seq)
        return speed_seq, drive_power_seq

    def _run_simulator(self, torque_request_seq, rspd_request_seq):
        """returns a dataframe"""
        simulator_inputs = self.trajectory.iloc[
            self.step_idx][self.config['simulator_fixed_input_cols']].to_dict(
            )  # load inputs from trajectory data
        simulator_outputs = []
        for (tq, rspd) in zip(torque_request_seq, rspd_request_seq):
            # iterate over 10ms for simulator results
            simulator_inputs.update({
                "IniDesChTarTq_Nm": tq,
                "IniDesChTarRotSpd_rpm": rspd,
            })

            result = self.simulator.step(
                simulator_inputs | {"BcuEnyMagtSoc_Inital": self.initial_soc}
            )  # TBD: simulation inputs and outputs, let output be a dict with array values for now
            simulator_outputs.append(result)  # a list of dicts with same keys

        simulator_outputs_df = pd.DataFrame(simulator_outputs)  # convert to df

        return simulator_outputs_df

    def get_obs_names(self):
        """
        Get observation names based on config and observation computation.

        Returns observation names in the same order as _compute_observation:
        - Sequential data: seq_<var>_mean, seq_<var>_std, seq_<var>_max, seq_<var>_median, seq_<var>_last
        - Non-sequential data: noseq_<var>
        - Simulator state vars: sim_<var>
        """
        names = []

        # Sequential data: 5 stats per variable (mean, std, max, median, last)
        seq_vars = self.config["state_variables"]["sequential"]
        suffixes = ['_mean', '_std', '_max', '_median', '_last']
        for var in seq_vars:
            for suffix in suffixes:
                names.append(f"seq_{var}{suffix}")

        # Non-sequential data
        noseq_vars = self.config["state_variables"]["non-sequential"]
        for var in noseq_vars:
            names.append(f"noseq_{var}")

        # Step size window data (future speed/accel stats)
        step_size_window_names = ['future_speed_mean', 'future_speed_std',
                                  'future_accel_mean', 'future_accel_std']
        names.extend(step_size_window_names)

        # Simulator state vars
        sim_vars = self.config['simulator_state_vars']
        for var in sim_vars:
            names.append(f"sim_{var}")

        return names

    # def _get_obs_names(self):
    #     #  shape=(self.config["state_variables"]["sequential"] * 3
    #     #                + len(self.config["state_variables"]["non-sequential"])
    #     #                + len(self.config['simulator_state_vars']), ))
    #     names = [col+'_mean' for col in self.config["state_variables"]["sequential"]] + \
    #             [col+'_std' for col in self.config["state_variables"]["sequential"]] + \
    #             [col+'_last' for col in self.config["state_variables"]["sequential"]] + \
    #             self.config["state_variables"]["non-sequential"] + \
    #             self.config['simulator_state_vars']
    #     return names
