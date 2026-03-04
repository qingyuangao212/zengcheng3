import pandas as pd
import numpy as np
from scipy.interpolate import RegularGridInterpolator

from reev_control.envs.utils import nvh_func

def step_fuel_consumption_reward(fc_seq, scale=1e-6):
    """
    EmsFuCns_seq: array, fuel consumption per ministep

    Note: a typical fc for 10ms is 1.67e-5, 
    """
    return - np.mean(fc_seq) / scale


def step_smooth_reward(power, last_power, lambda_smooth=1):
    return - lambda_smooth * np.sum((power - last_power) ** 2)


def step_soc_potential_reward(soc, soc_next, gamma, kappa):
    phi_now = kappa * soc
    phi_next = kappa * soc_next
    return gamma * phi_next - phi_now

def step_nvh_reward(tq_seq, n_seq, spd_seq):
    # Stack into shape (N, 3)
    query_points = np.stack(
        [np.asarray(spd_seq),
         np.asarray(tq_seq),
         np.asarray(n_seq)], axis=-1)
    nvh_score_seq = nvh_func(query_points) / 6 - 1  # scale to -1~1


    if np.any(np.isnan(nvh_score_seq)):
        print("Warning: NaN values in NVH score sequence. Check input data.")
        # find out if inputs are nan
        print("len of nvh_score_seq:", len(nvh_score_seq))
        print("num of nans:", np.sum(np.isnan(nvh_score_seq)))
        print("spd_seq:", spd_seq[:10])
        print("tq_seq:", tq_seq[:10])
        print("n_seq:", n_seq[:10])
        print("===========")

    # Call interpolation function
    return nvh_score_seq.sum()


def end_soc_reward(soc, done: bool, worst_penalty=-1e4):
    """
    1. soc在0-12或30-100：worst penalty
    2. 目标18, reward 0
    3. 12-18，18-30，线性差值
    注：<18的斜率为>18两倍
    
    """
    if not done:
        return 0

    if soc < 12 or soc > 30:
        return worst_penalty
    elif 12 <= soc < 18:
        return worst_penalty/6 * (18 - soc)
    elif 18 <= soc <= 30:
        return worst_penalty/12 * (soc - 18)

    return 0


# def step_soc_reward(soc_seq, UPPER_BOUND=80, LOWER_BOUND=12):
#     """行驶过程中12%≤SOC≤30%，无奖励，但SOC超出此区间，给超出部分二次惩罚"""
#     soc_seq = np.asarray(soc_seq)

#     upper_bound_loss = -np.dot(soc_seq > UPPER_BOUND, (soc_seq - UPPER_BOUND) * 1) / len(soc_seq)

#     lower_bound_loss = -np.dot(soc_seq < LOWER_BOUND, (LOWER_BOUND - soc_seq) * 2) / len(soc_seq)  # max -1.2

#     return lower_bound_loss + upper_bound_loss


def step_soc_reward(soc, UPPER_BOUND=75, LOWER_BOUND=15):
    """行驶过程中12%≤SOC≤30%，无奖励，但SOC超出此区间，给超出部分二次惩罚"""
    C = 2
    if soc < LOWER_BOUND:
        return -2 * C * (LOWER_BOUND - soc)**2
    elif soc > UPPER_BOUND:
        return -C * (soc - UPPER_BOUND)**2
    else:
        return 0.0  


def step_efficiency_reward(tq_seq, rspd_seq, fc_seq, dt_in_ms=10):

    if np.all(rspd_seq==0):
        return 0

    gen_energy_in_J = np.dot(tq_seq, rspd_seq)/9550 * dt_in_ms  # 总发电量， J= kW*ms
    F = np.sum(fc_seq)  # 总喷油量 = 每10ms喷油量之和

    if F==0:
        return 0    # this is redundant, but just to be safe
    else:
        eta = gen_energy_in_J / (F * 0.725 * 46) * 100   # 发电效率，单位：%

    eta_rescaled = np.interp(x=eta, xp=[0, 23.8, 31.2, 38.6, 50], fp=np.array([-1, -0.5, 0, 0.5, 1]))

    return eta_rescaled



def step_reward(sim_result: dict, done: bool, weights: list[float]|None=None):

    if weights is None:
        weights = [1, 1, 1, 10]

    r_fc = step_fuel_consumption_reward(sim_result['EmsFuCns'])
    # r_nvh = step_nvh_reward(sim_result['EmsEngTqFlywh'], sim_result['EmsEngSpd'], sim_result['speed_seq'])
    r_efficiency = step_efficiency_reward(sim_result['EmsEngTqFlywh'], sim_result['EmsEngSpd'], sim_result['EmsFuCns'])
    r_step_soc = step_soc_reward(sim_result['BcuEnyMagtSoc'][-1])
    r_end_soc = end_soc_reward(sim_result['BcuEnyMagtSoc'][-1], done)

    total_reward = np.dot(weights, [r_fc, r_efficiency, r_step_soc, r_end_soc])

    # this need to be hardcoded in callback.py and train code to make sure they get logged
    info = {"fc_reward": weights[0]*r_fc,
            "efficiency_reward": weights[1]*r_efficiency,
            "step_soc_reward": weights[2]*r_step_soc,
            "end_soc_reward": weights[3]*r_end_soc}

    return total_reward, info
