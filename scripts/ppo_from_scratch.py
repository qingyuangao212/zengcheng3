import gym
import torch
import torch.nn as nn
import numpy as np
from torch.distributions import Categorical

# Hyperparameters
GAMMA = 0.99
LAMBDA = 0.95
CLIP_EPS = 0.2
ENTROPY_COEF = 0.01
VALUE_COEF = 0.5
LR = 3e-4
BATCH_SIZE = 64
EPOCHS = 4
STEPS_PER_UPDATE = 2048

# Simple MLP for both policy and value
class PPOActorCritic(nn.Module):
    def __init__(self, obs_dim, act_dim):
        super().__init__()
        self.shared = nn.Sequential(
            nn.Linear(obs_dim, 64), nn.ReLU(),
            nn.Linear(64, 64), nn.ReLU()
        )
        self.policy_head = nn.Linear(64, act_dim)
        self.value_head = nn.Linear(64, 1)

    def forward(self, x):
        x = self.shared(x)
        return self.policy_head(x), self.value_head(x)

    def act(self, obs):
        logits, value = self.forward(obs)
        dist = Categorical(logits=logits)
        action = dist.sample()
        return action, dist.log_prob(action), dist.entropy(), value

# GAE advantage calculation
def compute_gae(rewards, values, dones, next_value):
    """gae IS A WEIGHTED SUM OF THE TD ERRORS"""
    advantages = []
    gae = 0
    values = values + [next_value]
    for t in reversed(range(len(rewards))):
        delta = rewards[t] + GAMMA * values[t+1] * (1 - dones[t]) - values[t]   # TD(1): r + gamma * V(s') - V(s)
        gae = delta + GAMMA * LAMBDA * (1 - dones[t]) * gae # recursive computation of GAE: A_t = delta + gamma * lambda * A_{t+1}
        advantages.insert(0, gae)   # insert at the beginning
    return advantages

# Training loop
def train():
    env = gym.make("CartPole-v1")
    obs_dim = env.observation_space.shape[0]
    act_dim = env.action_space.n

    model = PPOActorCritic(obs_dim, act_dim)
    optimizer = torch.optim.Adam(model.parameters(), lr=LR)

    obs = env.reset()
    episode_rewards = []

    for update in range(1000):
        storage = {'obs': [], 'actions': [], 'log_probs': [],
                   'rewards': [], 'dones': [], 'values': [], 'entropies': []}

        for _ in range(STEPS_PER_UPDATE):
            obs_tensor = torch.tensor(obs, dtype=torch.float32).unsqueeze(0)
            with torch.no_grad():
                action, log_prob, entropy, value = model.act(obs_tensor)

            next_obs, reward, done, _ = env.step(action.item())

            storage['obs'].append(obs_tensor.squeeze(0))
            storage['actions'].append(action)
            storage['log_probs'].append(log_prob)
            storage['rewards'].append(reward)
            storage['dones'].append(done)
            storage['values'].append(value.item())
            storage['entropies'].append(entropy)

            obs = next_obs
            if done:
                obs = env.reset()

        with torch.no_grad():
            next_obs_tensor = torch.tensor(obs, dtype=torch.float32).unsqueeze(0)
            _, _, _, next_value = model.act(next_obs_tensor)

        advantages = compute_gae(
            storage['rewards'], storage['values'], storage['dones'], next_value.item()
        )
        returns = [a + v for a, v in zip(advantages, storage['values'])]

        # Convert to tensors
        obs_batch = torch.stack(storage['obs'])
        act_batch = torch.stack(storage['actions'])
        logp_old = torch.stack(storage['log_probs']).detach()
        returns = torch.tensor(returns, dtype=torch.float32)
        advs = torch.tensor(advantages, dtype=torch.float32)
        advs = (advs - advs.mean()) / (advs.std() + 1e-8)

        # PPO update
        for _ in range(EPOCHS):
            idxs = np.arange(len(obs_batch))
            np.random.shuffle(idxs)
            for i in range(0, len(obs_batch), BATCH_SIZE):
                batch_idx = idxs[i:i+BATCH_SIZE]
                obs_b = obs_batch[batch_idx]
                act_b = act_batch[batch_idx]
                logp_old_b = logp_old[batch_idx]
                return_b = returns[batch_idx]
                adv_b = advs[batch_idx]

                logits, value = model(obs_b)
                dist = Categorical(logits=logits)
                logp = dist.log_prob(act_b)
                entropy = dist.entropy()

                ratio = torch.exp(logp - logp_old_b)
                clip_adv = torch.clamp(ratio, 1 - CLIP_EPS, 1 + CLIP_EPS) * adv_b
                policy_loss = -torch.min(ratio * adv_b, clip_adv).mean()

                value_loss = (return_b - value.squeeze()).pow(2).mean()
                entropy_loss = entropy.mean()

                loss = policy_loss + VALUE_COEF * value_loss - ENTROPY_COEF * entropy_loss

                optimizer.zero_grad()
                loss.backward()
                optimizer.step()

        print(f"Update {update} | Loss: {loss.item():.3f} | Return: {np.mean(storage['rewards']):.2f}")

    env.close()

if __name__ == "__main__":
    train()
