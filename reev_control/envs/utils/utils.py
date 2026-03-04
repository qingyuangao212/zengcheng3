__all__ = ["compute_drive_power", "efficiency_by_torque_and_rspd_spline", "min_rspd_by_power_spline", "initial_action_table", "spd_power_to_tq_rspd", "nvh_func"]

import numpy as np
import pandas as pd
from scipy.interpolate import UnivariateSpline, RectBivariateSpline, RegularGridInterpolator


"""
车辆动力学方程：
F（行驶阻力）=a+b*v+c*v^2(a=  158.9649   b=0.6238   c= 0.0455)
F（加速阻力）=ma(a为两个周期车速V的变化，m=2390)
驱动功率 P=FV=(F(行驶阻力)+F（加速阻力）)*V
"""
def compute_drive_power(speed, accel):
    a, b, c = 158.9649, 0.6238, 0.0455
    m = 2390  # kg

    # 行驶阻力
    F_rolling = a + b * speed + c * speed**2

    # 加速度 a = Δv / Δt
    F_accel = m * accel

    # 总阻力
    F_total = F_rolling + F_accel

    # 驱动功率 P = F * v
    power = F_total * speed

    return power


efficiency_by_torque_and_rspd_table = pd.read_excel('reev_control/envs/utils/车端增程器特性.xlsx', index_col=0)

efficiency_by_torque_and_rspd_spline = RectBivariateSpline(efficiency_by_torque_and_rspd_table.index,
                                                           efficiency_by_torque_and_rspd_table.columns,
                                                           efficiency_by_torque_and_rspd_table.values)

"""
发动机外特性表：对于每个转速的最大扭矩值
可以由此表计算对于每个转速的最大功率值
由此得出 每个功率对应的可允许最小转速，并插值为函数
"""
rspd_grid = np.array([900, 900, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500])
torque_grid = np.array([31.831, 110.28, 110.28, 141.89, 162.01, 183.85, 205.16, 217.18, 220, 220])
gen_power_grid = (torque_grid * rspd_grid * 2 * np.pi) / (60 * 1000)

min_rspd_by_power_spline = UnivariateSpline(gen_power_grid, rspd_grid, k=1, s=0)


# naive initial action
initial_action_table = np.array([
    [3.5, 10, 11.5, 12.5, 12.5, 13, 15, 20, 25, 28],
    [3.5, 10, 11.5, 13, 15, 17, 19, 24, 29, 32],
    [3.5, 11, 12, 15, 18, 20, 22, 27, 30, 33],
    [3.5, 11, 14, 16, 19, 21, 23, 28, 31, 34],
    [3.5, 11, 14, 17, 20, 22, 24, 30, 33, 36],
    [3.5, 12, 15, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 15, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 15, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 15, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 15, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 15, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 15, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 15, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 15, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 14.5, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 14.5, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 14.5, 18, 21, 23, 25, 30, 33, 36],
    [3.5, 12, 14.5, 18, 21, 23, 25, 30, 33, 36],
])

"""
用于简化动作空间求解rspd,tq rquests的函数
"""
from scipy.interpolate import RegularGridInterpolator
data = pd.read_excel('reev_control/envs/utils/发电功率-转速表.xlsx', index_col=0)
data.columns = data.columns.astype(float)
spd_power_to_rspd_interp = RegularGridInterpolator(
    points=(data.index.values, data.columns.values), 
    values=data.values
    )

def spd_power_to_tq_rspd(spd, power):
    """
    计算给定车速和功率下的转速和扭矩
    :param spd: 车速
    :param power: 功率
    :return: 转速和扭矩
    """
    rspd = spd_power_to_rspd_interp((spd, power))
    tq = 9_550 * power / rspd
    return tq, rspd



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
