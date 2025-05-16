import numpy as np
import pandas as pd
from scipy.interpolate import UnivariateSpline, RectBivariateSpline



# 车辆动力学方程：
# F（行驶阻力）=a+b*v+c*v^2(a=  158.9649   b=0.6238   c= 0.0455)
# F（加速阻力）=ma(a为两个周期车速V的变化，m=2390)
# 驱动功率 P=FV=(F(行驶阻力)+F（加速阻力）)*V

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
rspd_grid = np.array([900, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500])
torque_grid = np.array([110.28, 110.28, 141.89, 162.01, 183.85, 205.16, 217.18, 221.74, 219.74])
gen_power_grid = (torque_grid * rspd_grid * 2 * np.pi) / (60 * 1000)
min_rspd_by_power_spline = UnivariateSpline(gen_power_grid, rspd_grid, k=3, s=0)


# naive initial action 
initial_action_table = [
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
]

initial_nvh = 0


    