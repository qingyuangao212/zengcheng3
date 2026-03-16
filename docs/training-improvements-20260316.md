# Training Improvements - 20260316

## Overview
This document describes the training improvements applied to break through plateau and achieve better convergence.

## Problem
Training losses converged early and stopped improving:
- `value_loss` plateaued
- `policy_gradient_loss` stopped decreasing

## Solutions Applied

### 1. Hyperparameter Adjustments

| Parameter | Old | New | Reason |
|-----------|-----|-----|--------|
| learning_rate | 3e-4 | 1e-4 | More stable training, prevents overshooting |
| gae_lambda | 0.98 | 0.95 | More bias, faster learning |
| ent_coef | 0.05 | 0.01 | Less aggressive exploration |
| vf_coef | 0.25 | 0.15 | Focus more on policy learning |

### 2. Model Architecture

**Increased capacity**: 64 → 128 units per layer (4x more parameters)
- Policy network: [64, 64] → [128, 128]
- Value network: [64, 64] → [128, 128]

### 3. Training Improvements

- **ortho_init**: Enabled orthogonal initialization
  - Better gradient flow in deeper networks
  - Weights initialized as orthogonal matrix

- **optimizer**: Added AdamW option
  - Better regularization than Adam
  - Decouples weight decay from learning rate

## Implementation

```python
# In TRAIN_CONFIG
"learning_rate": 1e-4,
"gae_lambda": 0.95,
"ent_coef": 0.01,
"vf_coef": 0.15,
"use_ortho_init": True,
"use_adamw": True,

# In policy_kwargs
policy_kwargs = dict(
    squash_output=True,
    net_arch=[dict(pi=[128, 128], vf=[128, 128])],
    ortho_init=True,
)

# In optimizer_kwargs
optimizer_kwargs = dict(optimizer_class=torch.optim.AdamW)
```

## Notes

- Layer normalization not implemented (would require custom policy in SB3)
- ortho_init provides similar stability benefits to layer norm
- Reward/environment changes not included

## References

- PPO Paper: Schulman et al., 2017
- Orthogonal Initialization: Saxe et al., 2013
- AdamW: Loshchilov & Hutter, 2019
