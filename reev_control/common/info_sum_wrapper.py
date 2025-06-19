import gymnasium as gym

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

    def reset(self, **kwargs):
        self.info_sums = {key: 0.0 for key in self.info_keys}
        return self.env.reset(**kwargs)

    def step(self, action):
        obs, reward, terminated, truncated, info = self.env.step(action)

        # Accumulate specified info keys
        for key in self.info_keys:
            if key in info:
                self.info_sums[key] += info[key]

        # Add episode summary when done
        if terminated or truncated:
            for key in self.info_keys:
                info[key] = self.info_sums.get(key, 0.0)

        return obs, reward, terminated, truncated, info
