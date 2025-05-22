import gymnasium as gym
import numpy as np
from gymnasium.spaces import flatten, unflatten, flatten_space

class ActionFlatteningWrapper(gym.ActionWrapper):
    def __init__(self, env):
        super().__init__(env)
        self.original_action_space = env.unwrapped.action_space
        self.action_space = flatten_space(self.original_action_space)

    def action(self, action):
        # Convert flat action back to Dict format before passing to env
        return unflatten(self.original_action_space, action)

class InfoSumWrapper(gym.Wrapper):
    """
    A Gym environment wrapper that tracks and accumulates specified keys from the `info` dictionary 
    during an episode. When the episode ends (either by termination or truncation), the accumulated 
    values are added back into the final `info` dictionary under keys prefixed with "episode_".

    Example:
        If `info_keys = ['distance', 'energy']`, and the environment's `info` dict includes these keys
        during each step, the wrapper will accumulate their values. On episode end, the final `info`
        dict will contain:
            {
                'episode_distance': total_distance,
                'episode_energy': total_energy,
                ...
            }

    Args:
        env (gym.Env): The environment to wrap.
        info_keys (List[str]): A list of keys to track and sum from the `info` dict.
    """
    def __init__(self, env, info_keys):
        super().__init__(env)
        self.info_keys = info_keys
        self.info_sums = None
        self.steps = None

    def reset(self, **kwargs):
        self.info_sums = {key: 0.0 for key in self.info_keys}
        self.steps = 0
        return self.env.reset(**kwargs)

    def step(self, action):

        self.steps += 1
        # 
        obs, reward, terminated, truncated, info = self.env.step(action)

        # Accumulate specified info keys
        for key in self.info_keys:
            if key in info:
                self.info_sums[key] += info[key]

        # Add episode summary when done
        if terminated or truncated:
            for key in self.info_keys:
                info[key+"_sum"] = self.info_sums.get(key, 0.0)
                info[key+"_avg"] = info[key+"_sum"] / self.steps # Avoid division by zero

        return obs, reward, terminated, truncated, info

