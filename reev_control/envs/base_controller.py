import numpy as np
from scipy.interpolate import UnivariateSpline, RectBivariateSpline
from reev_control.envs.reward import nvh_func
from reev_control.envs.utils import efficiency_by_torque_and_rspd_spline, min_rspd_by_power_spline
import pandas as pd


# 最大转速：固定4500
MAX_RSPD=4500
MIN_RSPD=900


# =====================================================


class BaseController:
    """determine rsspd and torque by highest reward"""

    def __init__(self, speed_grid, drive_power_grid):

        self.speed_grid = speed_grid
        self.drive_power_grid = drive_power_grid
        self.reward_func = lambda x, y: efficiency_by_torque_and_rspd_spline(
            x, y, grid=False)

    # def env_interface(self, state, action, n_iters):

    def __call__(self, speed_seq, drive_power_seq, gen_power_table, min_nvh=6):
        best_torque_seq, best_rspd_seq = [], []

        power_request_seq = self.compute_power_request(speed_seq,
                                                       drive_power_seq,
                                                       gen_power_table)

        min_rspd_seq = min_rspd_by_power_spline(power_request_seq)

        min_rspd_seq = np.maximum(min_rspd_seq, MIN_RSPD)

        for speed, min_rspd, power_request in zip(speed_seq, min_rspd_seq,
                                                  power_request_seq):
            feasible_rspds = np.arange(min_rspd, MAX_RSPD, 10)

            feasible_torques = (power_request * 60 * 1000) / (2 * np.pi *
                                                              feasible_rspds)

            # Step 4: Compute NVH
            feasible_nvh = nvh_func(
                np.stack([
                    np.full_like(feasible_torques, speed), feasible_torques,
                    feasible_rspds
                ],
                         axis=-1))
            flags = feasible_nvh > min_nvh

            # if min_nvh_condition is satisfied for some feasible set of (torque, rspd), then select based on BEST REWARD
            # if not, select the best NVH
            if np.any(flags):
                feasible_rspds = feasible_rspds[flags]
                feasible_torques = feasible_torques[flags]
                rewards = self.reward_func(feasible_torques, feasible_rspds)
                best_index = np.argmax(rewards)

            else:
                # Fallback: use the pair with the best NVH
                best_index = np.argmax(feasible_nvh)

            best_torque = feasible_torques[best_index]
            best_rspd = feasible_rspds[best_index]

            best_torque_seq.append(best_torque)
            best_rspd_seq.append(best_rspd)

        return np.array(best_torque_seq), np.array(best_rspd_seq)

    def compute_power_request(self, speed_seq, drive_power_seq,
                              gen_power_table):
        # Vectorized approach using np.searchsorted
        speed_indices = np.searchsorted(self.speed_grid, speed_seq) - 1
        drive_power_indices = np.searchsorted(self.drive_power_grid,
                                              drive_power_seq) - 1

        # Use the indices to index into the gen_power_table
        gen_power_seq = gen_power_table[speed_indices, drive_power_indices]

        return gen_power_seq


class BaseControllerV2:
    """determine rspd and torque by highest reward"""
    def __init__(self):


        self.reward_func = lambda x, y: efficiency_by_torque_and_rspd_spline(x, y, grid=False)

    # def env_interface(self, state, action, n_iters):

    def __call__(self, speed_seq, power_request_seq, min_nvh=6):
        """The simple version where action outputs power request directly"""
        best_torque_seq, best_rspd_seq = [], []

        min_rspd_seq = min_rspd_by_power_spline(power_request_seq)

        for speed, min_rspd, power_request in zip(speed_seq, min_rspd_seq, power_request_seq):
            feasible_rspds = np.arange(min_rspd, MAX_RSPD, 10)
            feasible_torques = (power_request * 60 * 1000) / (2 * np.pi * feasible_rspds)

            # Step 4: Compute NVH
            feasible_nvh = nvh_func(np.stack([np.full_like(feasible_torques, speed), feasible_torques, feasible_rspds], axis=-1))
            flags = feasible_nvh > min_nvh

            # if min_nvh_condition is satisfied for some feasible set of (torque, rspd), then select based on BEST REWARD
            # if not, select the best NVH
            if np.any(flags):
                feasible_rspds = feasible_rspds[flags]
                feasible_torques = feasible_torques[flags]
                rewards = self.reward_func(feasible_torques, feasible_rspds)
                best_index = np.argmax(rewards)

            else:
                # Fallback: use the pair with the best NVH
                best_index = np.argmax(feasible_nvh)

            best_torque = feasible_torques[best_index]
            best_rspd = feasible_rspds[best_index]

            best_torque_seq.append(best_torque)
            best_rspd_seq.append(best_rspd)

        return np.array(best_torque_seq), np.array(best_rspd_seq)


    def compute_power_request(self, speed_seq, drive_power_seq, gen_power_table):
        # Vectorized approach using np.searchsorted
        speed_indices = np.searchsorted(self.speed_grid, speed_seq) - 1
        drive_power_indices = np.searchsorted(self.drive_power_grid, drive_power_seq) - 1

        # Use the indices to index into the gen_power_table
        gen_power_seq = gen_power_table[speed_indices, drive_power_indices]

        return gen_power_seq
