
import os
import yaml
import numpy as np
import pandas as pd
import gymnasium as gym
from gymnasium import spaces
# It's important to write out the module despite the evns/__init__.py.  Avoid circular imports
from reev_control.envs.trajectory_loader import TrajectoryLoader
from reev_control.envs.BaseController import BaseController
from reev_control.envs.simulator import Simulator   
from reev_control.envs.reward import step_reward

from reev_control.envs.utils import compute_drive_power
"""
需要給我的：
1. sequential和non-sequential的state variables
2. lowlevel控制器和simulator輸入輸出是什麽，什麽幀率，哪些是s'，哪些是reward function需要的
"""


class VehicleEnv(gym.Env):
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
                 data_folder,
                 config_path="reev_control/envs/config.yaml"):
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

        self.step_size_in_seconds = self.config.get('step_size_in_seconds', 1)
        self.step_size_in_10ms = 100 *  self.step_size_in_seconds

        self.obs_seq_len = self.config.get("obs_seq_len", 30)

        self.trajectory_loader = TrajectoryLoader(
            data_folder, step_size=self.step_size_in_seconds)

        self.base_controller = BaseController(
            speed_grid=self.config['action_space']['speed_grid'],
            drive_power_grid=self.config['action_space']['drive_power_grid']
            )

        self.simulator = Simulator(
            dll_path=self.config['simulator_model_path'])

        self.reward_fn = step_reward

       
        # Define observation space
        self.observation_space = spaces.Dict({
            "sequential":
            spaces.Box(
                low=-np.inf,
                high=np.inf,
                shape=(self.obs_seq_len,
                       len(self.config["state_variables"]["sequential"]))),
            "non-sequential":
            spaces.Box(
                low=-np.inf,
                high=np.inf,
                shape=(len(self.config["state_variables"]["non-sequential"])
                       + len(self.config['simulator_state_vars']), ))  
        })

        # Define action space
        self.action_space = spaces.Dict({
            "gen_power_grid":
            spaces.Box(low=0,
                       high=self.config['action_space']['gen_power_high'],
                       shape=( len(self.config['action_space']['speed_grid']),
                           len(self.config['action_space']['drive_power_grid']))),
            "min_nvh":
            spaces.Box(low=0.0, high=12.0, shape=())
        })

    def reset(self, seed=None):
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
        super().reset(seed=seed)
        self.trajectory = self.trajectory_loader.load_trajectory(
        )  # need to aggregate data by step size
        self.step_idx = self.config['data_start_index']


        # reinit vehicle simulator with random start_soc
        initial_soc = self.np_random.uniform(16, 20)
        self.simulator.reset(fixed_inputs={"BcuEnyMagtSoc_Inital": initial_soc})

        # Initialize self.state to all zeros except SOC
        initial_simulated_state = dict.fromkeys(self.config['simulator_state_vars'], 0)
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

        # 1.a compute speed every 10ms within the step: use the next step speed and assume constant acceleration
        speed_seq, drive_power_seq = self._compute_speed_and_power_seq()

        # 2. compute BASE CONTROLLER results: (tq, n) requests
        torque_request_seq, rspd_request_seq = self.base_controller(
            speed_seq=speed_seq,
            drive_power_seq=drive_power_seq,
            gen_power_table=action['gen_power_grid'],
            min_nvh=action['min_nvh'])
        
        info.update({"drive_power": drive_power_seq[-1], 
                     "torque_request": torque_request_seq[-1], 
                     "rspd_request": rspd_request_seq[-1]})

        assert (len(speed_seq) == len(drive_power_seq) == 
                len(torque_request_seq) == len(rspd_request_seq) == self.step_size_in_10ms)

        # ================= 3. SIMULATOR =================
        simulator_outputs_df = self._run_simulator(torque_request_seq, rspd_request_seq)

        # ================= 4. RL done, reward, state_prime =================
        # compute episode done flag
        done = (self.step_idx + 1 == len(self.trajectory) - 1)

        reward_inputs = simulator_outputs_df[self.config['simulator_reward_vars']].to_dict(orient='list')
        reward_inputs['speed_seq'] = speed_seq

        info.update(simulator_outputs_df.iloc[-1].to_dict())

        # compute step reward
        step_reward, reward_info = self.reward_fn(reward_inputs, done)

        info['done'] = done
        info.update(reward_info)

        self.step_idx += 1  # update step_idx must preceed compute_observation
        # compute s prime
        simulated_states = simulator_outputs_df.iloc[-1].loc[self.config['simulator_state_vars']].to_dict()
        self.state = self._compute_observation(simulated_states)

        truncated = False  # No truncation for now, for the general case where done can be set:  truncated = done and (self.step_idx + 1 < len(self.trajectory) - 1)

        return self.state, step_reward, done, truncated, info


    def _get_sequential_data(self):
        """sequential data are aggregated by minute, and padded to fixed length"""
        start_idx = max(0, self.step_idx - self.obs_seq_len + 1)    

        # draw sequential data and non-sequential data from trajectory, using step_index
        sequential_data = self.trajectory.iloc[start_idx:self.step_idx + 1][
            self.config["state_variables"]["sequential"]]
        
        # group sequential data by minute
        n_steps_per_min = 60 // self.step_size_in_seconds
        groups = np.arange(len(sequential_data)) // n_steps_per_min
        sequential_data = sequential_data.groupby(groups).mean()
        sequential_data = sequential_data.values

        # pad sequential data to obs_seq_len//n_steps_per_min
        if len(sequential_data) < self.obs_seq_len//n_steps_per_min:
            sequential_data = np.pad(
                sequential_data,
                ((self.obs_seq_len//n_steps_per_min - len(sequential_data), 0), (0, 0)),
                mode='constant', constant_values=0) # pad zeros so the row remain the same
            
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
        
        sequential_data = self._get_sequential_data()
    
        non_sequential_data = self._get_non_sequential_data()

        # Add simulator values (always update tq, n, soc before updating state)
        non_sequential_data = np.append(non_sequential_data,
                                        list(simulated_states.values()))

        return {
            "sequential": sequential_data.astype(np.float32),
            "non-sequential": non_sequential_data.astype(np.float32)
        }

    def _compute_speed_and_power_seq(self):

        start_speed = self.trajectory['EspVehSpd'].iloc[self.step_idx]
        end_speed = self.trajectory['EspVehSpd'].iloc[self.step_idx + 1]  # assertion above guarantee validity here
        speed_seq = np.linspace(start_speed, end_speed, self.step_size_in_10ms + 1)[:-1]

        # 1.b compute drive_power every 10ms （发动机外特性查表）
        constant_accel = (end_speed - start_speed) / self.step_size_in_seconds
        drive_power_seq = compute_drive_power(speed_seq, constant_accel)
        return speed_seq, drive_power_seq

    def _run_simulator(self, torque_request_seq, rspd_request_seq):

        simulator_inputs = self.trajectory.iloc[self.step_idx][self.config['simulator_fixed_input_cols']].to_dict()   # load inputs from trajectory data
        simulator_outputs = []
        for (tq, rspd) in zip(torque_request_seq, rspd_request_seq):
            # iterate over 10ms for simulator results
            simulator_inputs.update({
                "IniDesChTarTq_Nm": tq,
                "IniDesChTarRotSpd_rpm": rspd,
            })
            result = self.simulator.step(simulator_inputs)  # TBD: simulation inputs and outputs, let output be a dict with array values for now
            simulator_outputs.append(result)    # a list of dicts with same keys

        simulator_outputs_df = pd.DataFrame(simulator_outputs)  # convert to df
        
        simulator_outputs_df['EmsEngTqFlywh'] = simulator_outputs_df['EmsEngTqFlywh'].abs()
        simulator_outputs_df['EmsFuCns'] = simulator_outputs_df['EmsFuCns'].abs()

        return simulator_outputs_df

    def _get_obs_names(self):
    #  shape=(self.config["state_variables"]["sequential"] * 3 
    #                + len(self.config["state_variables"]["non-sequential"])
    #                + len(self.config['simulator_state_vars']), ))  
        names = self.config["state_variables"].copy()
        names["non-sequential"] += self.config['simulator_state_vars']
        return names