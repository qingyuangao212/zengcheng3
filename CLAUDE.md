# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Package Manager

**Always use `uv` for all package management.** Do not use `pip` or `python` directly.

## Project Overview

This is a **reinforcement learning project** for Range Extended Electric Vehicle (REEV) control. It uses stable-baselines3 (PPO/SAC) to train agents that control vehicle behavior based on simulator data.

## Common Commands

### Training

```bash
# Train PPO on env4 (main entry point)
uv run scripts/sb3_ppo_train_env4.py --run "my_run_name"

# Train with custom device
uv run scripts/sb3_ppo_train_env4.py --device cuda:0

# Resume training
uv run scripts/resume_training.py --model-path train_results/.../model.zip

# Train with feature extractor
uv run scripts/sb3_ppo_train_fe.py
```

### Evaluation

```bash
# Evaluate trained model
uv run scripts/evaluate_v3.py --model-path <path_to_model> --n-episodes 10
```

### Inference

```bash
# Run inference with feature extractor
uv run scripts/inference_fe.py
```

### Installation

```bash
# Install package in development mode (always use uv)
uv pip install -e .
```

## Architecture

### Package Structure

```
reev_control/
├── envs/                    # Environment implementations
│   ├── simple_vehicle_env4.py      # Main environment (Box action space)
│   ├── simple_vehicle_env4_fe.py   # With feature extractor
│   ├── wrappers.py                # Env wrappers (InfoHistoryWrapper, etc.)
│   ├── reward.py                  # Reward functions
│   └── simulator.py               # CTypes wrapper for MATLAB simulator
├── common/                  # Shared utilities
│   ├── callbacks.py              # WandbCallbackWithVecNorm, AdvantageLoggingCallback
│   ├── feature_extractor.py     # LSTMFeatureExtractor
│   └── lr_schedule.py            # Learning rate schedules
├── custom_ppo.py            # CustomPPO class (extends SB3 PPO)
└── custom_sac.py            # CustomSAC class
```

### Training Flow

1. **Entry point**: `scripts/sb3_ppo_train_env4.py` defines `make_env()` factory
2. **Environment**: `SimpleVehicleEnv4` loads CSV trajectory data, wraps with `InfoHistoryWrapper` + `Monitor`
3. **Vectorization**: `SubprocVecEnv` for parallel envs + `VecNormalize` for observation normalization
4. **Model**: `CustomPPO` with `MlpPolicy` and optional `LSTMFeatureExtractor`
5. **Logging**: Weights & Biases (project: `reev_control`) + tensorboard

### Key Patterns

- **Environment versions**: Multiple `SimpleVehicleEnv` variants (v1-v4) with different action/observation spaces. env4 uses flat `Box` action space.
- **Reward weights**: Defined in training scripts as `[fc_weight, efficiency_weight, step_soc_weight, end_soc_weight]`
- **Simulator**: CTypes wrapper in `envs/simulator.py` calling MATLAB-compiled shared library

## Configuration

Training configs are defined in each script (e.g., `env_config`, `train_config` dicts). Key parameters:

- `obs_seq_len`: 600 (observation sequence length in timesteps)
- `step_size_in_seconds`: 10
- `n_envs`: 8 (parallel environments)
- `total_timesteps`: 5,000,000
- `gamma`: 0.99
- `gae_lambda`: 0.95
- `vf_coef`: 0.15

## Known Issues

- `InfoLogCallback` in `reev_control/common/callbacks.py` has potential issues with numpy operations
- Duplicate `InfoSumWrapper` exists in both `envs/wrappers.py` and `common/info_sum_wrapper.py`

## Network Access & VPN

This machine is on a corporate network with firewall restrictions. Some external sites (Google, Hugging Face, etc.) may be blocked.

### When WebFetch or WebSearch Fails Due to Firewall

If `WebFetch` or `WebSearch` tools cannot access a URL (timeout, 000, or connection error):

1. **Start the MonoCloud VPN** using the startup script:
   ```bash
   ~/memory/start-vpn.sh start
   ```

2. **Use `curl` with proxy** instead of `WebFetch`:
   ```bash
   # Option A: Use -x flag (one command only)
   curl -x http://127.0.0.1:7890 -s https://blocked-site.com/content > ~/downloads/content.html

   # Option B: Use env prefix (one command only)
   https_proxy=http://127.0.0.1:7890 curl -s https://blocked-site.com/content > ~/downloads/content.html

   # Option C: Export for multiple commands in same session
   export http_proxy=http://127.0.0.1:7890
   export https_proxy=http://127.0.0.1:7890
   export all_proxy=socks5://127.0.0.1:7891
   curl -s https://blocked-site.com/content > ~/downloads/content.html
   ```

3. **Then use Claude's `Read` tool** to analyze the downloaded file.

### VPN Management Commands

```bash
~/memory/start-vpn.sh start    # Start VPN
~/memory/start-vpn.sh stop     # Stop VPN
~/memory/start-vpn.sh restart  # Restart VPN
~/memory/start-vpn.sh status   # Check status
```

### Switching Proxy Servers (Optional)

Mihomo provides multiple server locations. To switch between them:

```bash
# List available servers
curl -s http://127.0.0.1:19090/proxies | jq '.proxies | keys'

# Switch to a specific server (e.g., Singapore instead of Hong Kong)
curl -X PUT http://127.0.0.1:19090/proxies/Proxy -d '{"name":"🇸🇬 Relay-SG1"}'
```
