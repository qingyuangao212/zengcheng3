import wandb
import os
from collections import defaultdict
from wandb.integration.sb3 import WandbCallback
import logging
import json

class InfoLogCallback(WandbCallback):
    """
    Callback that logs mean values of specified info keys at the end of each rollout.
    """

    METRIC_KEYS = [
        'fc_reward', 'efficiency_reward', 'step_soc_reward', 'end_soc_reward', 'action_norm'
    ]

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


logger = logging.getLogger(__name__)


class WandbCallbackWithVecNorm(WandbCallback):
    """Add saving and logging vecnorm env"""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.vecnormalize_path = os.path.join(self.model_save_path,
                                              "vec_env.pkl")

    def save_model(self) -> None:
        # call parent class method to save the model + wandb logging
        super().save_model()

        # save VecNormalize env if exists
        vec_norm_env = self.model.get_vec_normalize_env()

        if vec_norm_env is not None:
            vec_norm_env.save(self.vecnormalize_path)
            wandb.save(self.vecnormalize_path, base_path=self.model_save_path)

            # # optionally save obs_rms stats as JSON
            # obs_rms_dict = {
            #     "mean": vec_norm_env.obs_rms.mean.tolist(),
            #     "var": vec_norm_env.obs_rms.var.tolist()
            # }
            # json_path = os.path.join(self.model_save_path,
            #                          "vecnorm_params.json")
            # with open(json_path, "w") as f:
            #     json.dump(obs_rms_dict, f, indent=4)
            # wandb.save(json_path, base_path=self.model_save_path)

        if self.verbose > 1:
            logger.info(
                f"Saving vecnormalize env checkpoint to {self.vecnormalize_path}"
            )


    # def save_model(self) -> None:
    #     self.model.save(self.path)
    #     wandb.save(self.path, base_path=self.model_save_path)

    #     vec_norm_env = self.model.get_vec_normalize_env()
    #     if vec_norm_env is not None:
    #         vec_norm_env.save(self.vecnormalize_path)

    #     wandb.save(self.vecnormalize_path,
    #                base_path=self.model_save_path)  # sync to wandb folder?

    #     # save the obs_rms stats
    #     obs_rms_dict = {
    #         "mean": self.model.env.obs_rms.mean.tolist(),
    #         "var": self.model.env.obs_rms.var.tolist()
    #     }
    #     with open(os.path.join(self.model_save_path, "vecnorm_params.json"), "w") as f:
    #         json.dump(obs_rms_dict, f, indent=4)
    #     wandb.save(os.path.join(self.model_save_path, "vecnorm_params.json"),
    #                base_path=self.model_save_path)

    #     if self.verbose > 1:
    #         logger.info(f"Saving model checkpoint to {self.path}")
    #         logger.info(
    #             f"Saving vecnormalize env checkpoint to {self.vecnormalize_path}"
    #         )
