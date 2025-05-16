
"""
step reward
"""
import pandas as pd
import numpy as np
from scipy.interpolate import RegularGridInterpolator

def interpolate_nvh_func():
    file_path = '/data2/qingyuan/projects/zengcheng3/data/params/智能增程3.0_NVH奖励、惩罚设计.xlsx'
    sheet_names = pd.ExcelFile(file_path).sheet_names

    sheets_to_read = sheet_names[:-2]
    dfs = pd.read_excel(file_path, sheet_name=sheets_to_read)

    processed_dfs = {}
    for sheet_name, df in dfs.items():
        df = df.iloc[:, 1:]  
        df.set_index(df.columns[0], inplace=True)  
        processed_dfs[sheet_name] = df  
        
    # Define sheet name mapping to numerical ranges
    sheet_intervals = {
        '0-30': (0, 30), '30-40': (30, 40), '40-50': (40, 50), '50-60': (50, 60),
        '60-70': (60, 70), '70-80': (70, 80), '80-90': (80, 90), '90-100': (90, 100),
        '100-110': (100, 110), '120': (110, 120), '>120': (120, 200)  # 120 → 110-120
    }

    # Convert sheets into a numerical list
    speed_values = []
    grid_data = []  # Store all data as 3D array

    for sheet_name, df in processed_dfs.items():
        start, end = sheet_intervals[sheet_name]
        speed_values.append(start)

        # Convert DataFrame to numpy array (ensure consistency)
        grid_data.append(df.to_numpy(dtype=np.float64))  # Convert to float for interpolation

        if start != end:  # If it's a range, duplicate the values at `end`
            speed_values.append(end-1e-5)
            grid_data.append(df.to_numpy(dtype=np.float64))

    # Convert lists to numpy arrays
    speed_values = np.array(speed_values)
    grid_data = np.array(grid_data)  # Shape: (num_speeds, num_x, num_y)

    # Get X and Y axis values from index & columns
    x_values = processed_dfs[next(iter(processed_dfs))].index.to_numpy(dtype=np.float64)
    y_values = processed_dfs[next(iter(processed_dfs))].columns.to_numpy(dtype=np.float64)

    interp_func = RegularGridInterpolator((speed_values, x_values, y_values), grid_data, method='linear', bounds_error=False, fill_value=None)
    
    return interp_func

nvh_func = interpolate_nvh_func()

def step_nvh_reward(tq_seq, n_seq, spd_seq):    
    # Stack into shape (N, 3)
    query_points = np.stack([np.asarray(spd_seq), np.asarray(tq_seq), np.asarray(n_seq)], axis=-1)
    nvh_score_seq = nvh_func(query_points)/6 - 1  # scale to -1~1

    # Call interpolation function
    return nvh_score_seq.sum()


def end_soc_reward(soc, done: bool, worst_penalty=-1e-4):
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


def step_soc_reward(soc_seq, done: bool, MAX_BOUND=30, MIN_BOUND=12):
    """行驶过程中12%≤SOC≤80%，无奖励，但SOC超出此区间，给超出部分二次惩罚"""
    if done:
        return 0
    
    if max(soc_seq) > MAX_BOUND:
        return -(max(soc_seq) - MAX_BOUND)**2
    elif min(soc_seq) < MIN_BOUND:
        return -(MIN_BOUND - min(soc_seq))**2
    
    return 0



def step_efficiency_reward(tq_seq, rspd_seq, EmsFuCns_seq, dt_in_ms=10):
    """accept sequence of 发电功率 and 喷油量 """
    # TBD
    gen_energy_in_J = np.dot(tq_seq, rspd_seq) * dt_in_ms # 总发电量， J= kW*ms
    F = np.sum(EmsFuCns_seq)    # 总喷油量 = 每10ms喷油量之和
    eta = gen_energy_in_J / (F * 0.725 /46)
    return eta
    
    
    
def step_reward(simulation_result, done):
    return

# def step_reward(soc_seq, tq_seq, n_seq, spd_seq, done, step_power_generation, step_gas_consumption, weights=[1,1,1,1]):
#     return np.dot(weights, 
#                   [step_soc_reward(soc_seq, done), 
#                    step_nvh_reward(tq_seq, n_seq, spd_seq), 
#                    step_efficiency_reward(step_power_generation, step_gas_consumption), 
#                    end_soc_reward(soc_seq[-1], done)])
    
