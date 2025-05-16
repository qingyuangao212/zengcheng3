from scripts.sb3_ppo_train import *
from stable_baselines3.common.env_checker import check_env

env = make_env()()
obs, info = env.reset()
action = env.action_space.sample()
state, step_reward, done, _, step_info = env.step(action)
# print(obs)
check_env(env, warn=True)




