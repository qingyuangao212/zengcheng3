import pandas as pd
import numpy as np
from scipy.interpolate import RegularGridInterpolator


def interpolate_nvh_func():

    file_path = 'reev_control/envs/utils/智能增程3.0_NVH奖励、惩罚设计_20250520.xlsx'
    sheet_names = pd.ExcelFile(file_path).sheet_names

    sheets_to_read = sheet_names[1:-2]
    dfs = pd.read_excel(file_path, sheet_name=sheets_to_read)

    processed_dfs = {}
    for sheet_name, df in dfs.items():
        df = df.iloc[:, 1:]
        df.set_index(df.columns[0], inplace=True)
        processed_dfs[sheet_name] = df

    # Define sheet name mapping to numerical ranges
    sheet_intervals = {
        '0-30': (0, 30),
        '30-40': (30, 40),
        '40-50': (40, 50),
        '50-60': (50, 60),
        '60-70': (60, 70),
        '70-80': (70, 80),
        '80-90': (80, 90),
        '90-100': (90, 100),
        '100-110': (100, 110),
        '120': (110, 120),
        '>120': (120, 200)  
    }

    # Convert sheets into a numerical list
    speed_values = []
    grid_data = []  # Store all data as 3D array

    for sheet_name, df in processed_dfs.items():
        start, end = sheet_intervals[sheet_name]
        speed_values.append(start)

        # Convert DataFrame to numpy array (ensure consistency)
        grid_data.append(df.to_numpy())  # Convert to float for interpolation

        if start != end:  # If it's a range, duplicate the values at `end`
            speed_values.append(end - 1e-4)
            grid_data.append(df.to_numpy())

    # Convert lists to numpy arrays
    speed_values = np.array(speed_values)
    grid_data = np.array(grid_data)  # Shape: (num_speeds, num_x, num_y)

    # Get X and Y axis values from index & columns
    x_values = processed_dfs[next(
        iter(processed_dfs))].index.to_numpy(dtype=np.float64)
    y_values = processed_dfs[next(
        iter(processed_dfs))].columns.to_numpy(dtype=np.float64)

    interp_func = RegularGridInterpolator((speed_values, x_values, y_values),
                                          grid_data,
                                          method='linear',
                                          bounds_error=False,
                                          fill_value=None)

    return interp_func

nvh_func = interpolate_nvh_func()


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

def step_soc_reward(soc_seq, UPPER_BOUND=30, LOWER_BOUND=12):
    """行驶过程中12%≤SOC≤80%，无奖励，但SOC超出此区间，给超出部分二次惩罚"""
    soc_seq = np.asarray(soc_seq)
    upper_bound_loss = -np.dot(soc_seq > UPPER_BOUND, (soc_seq - UPPER_BOUND)*0.05)
    # upper_bound_loss = -np.dot(soc_seq > UPPER_BOUND, (soc_seq - UPPER_BOUND)**1.5*1e-2)

    lower_bound_loss = -np.dot(soc_seq < LOWER_BOUND, (LOWER_BOUND - soc_seq)*0.1)  # max -1.2

    return upper_bound_loss + lower_bound_loss



def step_efficiency_reward(tq_seq, rspd_seq, EmsFuCns_seq, dt_in_ms=10):

    if np.all(rspd_seq==0):
        return 0

    gen_energy_in_J = np.dot(tq_seq, rspd_seq)/9550 * dt_in_ms  # 总发电量， J= kW*ms
    F = np.sum(EmsFuCns_seq)  # 总喷油量 = 每10ms喷油量之和

    if F==0:
        return 0
    else:
        eta = gen_energy_in_J / (F * 0.725 * 46000) * 100 / 1000  # 发电效率，单位：%

    eta_rescaled = np.interp(x=eta, xp=[0, 23.8, 31.2, 38.6, 50], fp=np.array([-1, -0.5, 0, 0.5, 1])/2)

    return eta_rescaled



def step_reward(sim_result: dict, done: bool, weights: list[float]|None=None):

    if weights is None:
        weights = [0.5, 0.2, 0.15, 0.15]

    r_nvh = step_nvh_reward(sim_result['EmsEngTqFlywh'], sim_result['EmsEngSpd'], sim_result['speed_seq'])
    r_efficiency = step_efficiency_reward(sim_result['EmsEngTqFlywh'], sim_result['EmsEngSpd'], sim_result['EmsFuCns'])
    r_step_soc = step_soc_reward(sim_result['BcuEnyMagtSoc'])
    r_end_soc = end_soc_reward(sim_result['BcuEnyMagtSoc'][-1], done)

    total_reward = np.dot(weights, [r_nvh, r_efficiency, r_step_soc, r_end_soc])

    info = {"nvh_reward": weights[0]*r_nvh,
            "efficiency_reward": weights[1]*r_efficiency,
            "step_soc_reward": weights[2]*r_step_soc,
            "end_soc_reward": weights[3]*r_end_soc}

    return total_reward, info
