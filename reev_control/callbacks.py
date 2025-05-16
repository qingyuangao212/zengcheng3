import wandb
from collections import defaultdict
from wandb.integration.sb3 import WandbCallback


class InfoLogCallback(WandbCallback):
    """
    Callback that logs mean values of specified info keys at the end of each rollout.
    """

    METRIC_KEYS = ['efficiency_reward', 'step_soc_reward', 'end_soc_reward', 'action_norm']

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.info_buffer = defaultdict(list)

    def _on_step(self) -> bool:
        infos = self.locals.get("infos", [])

        # Normalize to list for both single and vectorized environments
        if isinstance(infos, dict):
            infos = [infos]

        for info in infos:
            for key in self.METRIC_KEYS:
                if key in info:
                    self.info_buffer[key].append(info[key])

        return super()._on_step()

    def _on_rollout_end(self) -> None:
        """
        Log mean of buffered metrics and clear the buffer.
        """
        if self.info_buffer:
            rollout_means = {
                f"rollout/{key}_mean": sum(values) / len(values)
                for key, values in self.info_buffer.items() if values
            }
            wandb.log(rollout_means)

        self.info_buffer.clear()
        return super()._on_rollout_end()