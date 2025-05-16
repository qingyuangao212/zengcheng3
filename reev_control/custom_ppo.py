import sys
import time
import numpy as np
from stable_baselines3 import PPO
from stable_baselines3.common.utils import safe_mean

class CustomPPO(PPO):
    def __init__(self, *args, info_keys=None, **kwargs):
        """
        Modified from stable_baselines3 OnPolicyAlgorithm.dump_logs.
        Log other keys from ep_info_buffer

        :param info_keys: List of custom episode-level keys to record (e.g. ['energy', 'distance']).
        """
        super().__init__(*args, **kwargs)
        
        self.info_keys = info_keys or ["nvh_reward", "efficiency_reward", "step_soc_reward", "end_soc_reward"]   # use env info_keys (which are logged to ep_info_buffer) if not provided

    def dump_logs(self, iteration: int = 0) -> None:
        """
        Write log including custom info_keys.
        """
        assert self.ep_info_buffer is not None
        assert self.ep_success_buffer is not None

        time_elapsed = max((time.time_ns() - self.start_time) / 1e9, sys.float_info.epsilon)
        fps = int((self.num_timesteps - self._num_timesteps_at_start) / time_elapsed)

        if iteration > 0:
            self.logger.record("time/iterations", iteration, exclude="tensorboard")

        if len(self.ep_info_buffer) > 0 and len(self.ep_info_buffer[0]) > 0:
            self.logger.record("rollout/ep_rew_mean", safe_mean([ep_info["r"] for ep_info in self.ep_info_buffer]))
            self.logger.record("rollout/ep_len_mean", safe_mean([ep_info["l"] for ep_info in self.ep_info_buffer]))

            # Record custom info_keys
            for key in self.info_keys:
                self.logger.record(f"rollout/ep_{key}_mean", safe_mean([ep_info[key] for ep_info in self.ep_info_buffer]))


        self.logger.record("time/fps", fps)
        self.logger.record("time/time_elapsed", int(time_elapsed), exclude="tensorboard")
        self.logger.record("time/total_timesteps", self.num_timesteps, exclude="tensorboard")

        # other metrics
        self.logger.record("rollout/num_episodes", np.sum([loader.trajectory_counter for loader in self.get_env().get_attr("trajectory_loader")]))


        if len(self.ep_success_buffer) > 0:
            self.logger.record("rollout/success_rate", safe_mean(self.ep_success_buffer))

        self.logger.dump(step=self.num_timesteps)
