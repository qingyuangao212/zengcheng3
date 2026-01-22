"""2025/05/22A version with simplified action space: power request only
Instead of searching over feasible rspd and tq for best efficiency, use a predefined function to comute rspd and tq requests

2025/06/09 add start/stop action, add file_list_file integration

2025/06/18 add feature extractor: envrionment observation space is a Dict with full sequential data
"""

import os
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

from reev_control.envs.utils import compute_drive_power, spd_power_to_tq_spd


class SimpleVehicleEnv3FE(gym.Env):
    """
    Custom Gymnasium environment for REEV engine control.
    
    The engine control is implemented in the following way: tbd

    The environment loads vehicle trajectory data from CSV files and combines it with 
    a simulator to model the system's response to actions. Observations consist of sequential 
    time-series data and state variables. A low-level controller processes actions to generate 
    torque and speed values, which the simulator then uses to compute additional state updates.

    Attributes:
        trajectory_loader (TrajectoryLoader): Loader for reading trajectory CSV files.
        simulator: Simulator for computing system dynamics.
        reward_fn (function): External function for computing rewards.
        config (dict): Configuration loaded from a YAML file.
        obs_seq_len (int): Number of sequential time steps in the observation.
        observation_space (gymnasium.spaces.Dict): Observation space definition.
        action_space (gymnasium.spaces.Box): Action space definition.
        trajectory (pd.DataFrame): Current trajectory data.
        step_idx (int): Current step index within the trajectory.
        tq (float): Torque value from the low-level controller.
        n (float): Speed value from the low-level controller.
        soc (float): State of charge (initially from trajectory data).
        state (dict): Current observation containing sequential and state data.
    """

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
        self.seed = self.config.get('seed')

        self.step_size_in_seconds = self.config.get('step_size_in_seconds', 1)
        self.step_size_in_10ms = 100 * self.step_size_in_seconds

        self.obs_seq_len = self.config.get("obs_seq_len", 30)

        self.trajectory_loader = TrajectoryLoader(
            data_folder=data_folder,
            step_size=self.step_size_in_seconds,
            min_length=self.config.get('data_min_length', 1800),
            file_list_file=self.config.get('file_list_file', None),  # optional file list
            seed=self.config.get('seed')  # manages shuffling of data files; if not passed just random shuffle
        )

        self.base_controller = None

        self.simulator = Simulator(
            dll_path=self.config['simulator_model_path'])

        self.reward_fn = lambda *args: step_reward(
            *args, self.config.get("reward_weights", [0.5, 0.2, 0.15, 0.15]
                                   ))  # allow setting reward weights

        # Define observation space
        self.observation_space = gym.spaces.Dict({
            "sequential": gym.spaces.Box(
                low=-np.inf,
                high=np.inf,
                shape=(self.obs_seq_len, len(self.config["state_variables"]["sequential"])),
                dtype=np.float32
            ),
            "non-sequential": gym.spaces.Box(
                low=-np.inf,
                high=np.inf,
                shape=(len(self.config["state_variables"]["non-sequential"]) +
                   len(self.config['simulator_state_vars']),),
                dtype=np.float32
            )
        })

        # Define action space
        self.action_space = spaces.Box(
            low=np.array([self.config['action_space']['gen_power_low'], 0]),    
            high=np.array([self.config['action_space']['gen_power_high'], 1]),  
            dtype=np.float32
        )   # action[0] power, action[1] start/stop (0 or 1)

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
        super().reset(seed=self.seed, options=options)  # when seed is none this doesn't change the np_random seed
        self.trajectory = self.trajectory_loader.load_trajectory()  # need to aggregate data by step size
        self.step_idx = self.config['data_start_index']

        # reinit vehicle simulator with random start_soc
        initial_soc = self.np_random.uniform(16, 20)
        self.simulator.reset(
            fixed_inputs={"BcuEnyMagtSoc_Inital": initial_soc})

        # Initialize self.state to all zeros except SOC
        initial_simulated_state = dict.fromkeys(
            self.config['simulator_state_vars'], 0)
        initial_simulated_state['BcuEnyMagtSoc'] = initial_soc

        self.state = self._compute_observation(initial_simulated_state)

        return self.state, {"BcuEnyMagtSoc": initial_soc}

    def step(self, action):
        """
        RL action是一个给下游控制器查表用的速度，驱动功率对应的表格
        Step包括以下步骤：
        1. 计算在一个RL step中对应的10ms单位速度和驱动功率序列, 作为BaseController输入
        2. 计算BaseController结果：基于action中的RL规划的发电功率表格，最小NVH限制；输出：10ms单位扭矩转速请求序列
        3. 计算Simulator结果：simulator先load当前step的traj变量，然后Iterate over第二步计算的扭矩转速请求序列，输出真实扭矩转速序列
        4. 计算reward, next state
        """
        info = {}

        assert (self.step_idx < len(self.trajectory) - 1)
        assert (self.step_idx + self.step_size_in_seconds
                < len(self.trajectory))
        
        # 1.a compute speed every 10ms within the step: use the next step speed and assume constant acceleration
        speed_seq, drive_power_seq = self._compute_speed_and_power_seq()

        # parse action
        engine_stop = action[1] >= 0.5 
        constant_power_request = action[0]

        if engine_stop:
            # action[1] == 0 means stop, set speed to 0
            power_request_seq = np.zeros_like(speed_seq)
            torque_request_seq, rspd_request_seq = np.zeros_like(speed_seq), np.zeros_like(speed_seq)

        else:   # action[1] >= 0.5
            power_request_seq = np.tile(constant_power_request, len(speed_seq))  # constant
            torque_request_seq, rspd_request_seq = spd_power_to_tq_spd(speed_seq, power_request_seq)

        info.update({
            "start_speed": speed_seq[0],
            "end_speed": speed_seq[-1],
            "drive_power": drive_power_seq[-1],
            "action.engine_stop": engine_stop,
            "action.power_request": np.nan if engine_stop else power_request_seq[-1],
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

        info.update(simulator_outputs_df.iloc[-1].to_dict())

        # compute step reward
        step_reward, reward_info = self.reward_fn(reward_inputs, done)

        info['done'] = done
        info.update(reward_info)

        self.step_idx += self.step_size_in_seconds  # update step_idx must preceed compute_observation
        # compute s prime
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

        return sequential_data

    def _get_non_sequential_data(self):

        non_sequential_data = self.trajectory.iloc[self.step_idx][
            self.config["state_variables"]["non-sequential"]].values

        return non_sequential_data

    def _compute_observation(self, simulated_states: dict):
        """
        Computes the current observation.

        The observation includes:
        - Sequential data: Past `obs_seq_len` steps of selected state variables.
        - State data: Current step's state variables plus additional computed values.

        Returns:
            dict: Dictionary containing 'sequential' and 'state' keys.
        """

        sequential_data = self._get_sequential_data().astype(np.float32)

        non_sequential_data = self._get_non_sequential_data()

        # Add simulator values (always update tq, n, soc before updating state)
        non_sequential_data = np.append(non_sequential_data, list(simulated_states.values())).astype(np.float32)

        obs = {
            "sequential": sequential_data,
            "non-sequential": non_sequential_data
        }
        
        return obs

    def _compute_speed_and_power_seq(self):

        start_speed = self.trajectory['EspVehSpd'].iloc[self.step_idx]
        end_speed = self.trajectory['EspVehSpd'].iloc[
            self.step_idx + 1]  # assertion above guarantee validity here
        speed_seq = np.linspace(start_speed, end_speed,
                                self.step_size_in_10ms + 1)[:-1]

        # 1.b compute drive_power every 10ms （发动机外特性查表）
        constant_accel = (end_speed - start_speed) / self.step_size_in_seconds
        drive_power_seq = compute_drive_power(speed_seq, constant_accel)
        return speed_seq, drive_power_seq

    def _run_simulator(self, torque_request_seq, rspd_request_seq):

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
                simulator_inputs
            )  # TBD: simulation inputs and outputs, let output be a dict with array values for now
            simulator_outputs.append(result)  # a list of dicts with same keys

        simulator_outputs_df = pd.DataFrame(simulator_outputs)  # convert to df

        simulator_outputs_df['EmsEngTqFlywh'] = simulator_outputs_df[
            'EmsEngTqFlywh'].abs()
        simulator_outputs_df['EmsFuCns'] = simulator_outputs_df[
            'EmsFuCns'].abs()

        return simulator_outputs_df

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
