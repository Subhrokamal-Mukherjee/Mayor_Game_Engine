# MAYOR Engine v2 (Ground-Up Python Rebuild)

A clean-slate, portability-first simulation platform where the player acts as mayor.

## Product Direction
Two-learning-engine architecture plus one narrative layer:

1. **Economic Engine (NN + RL training target)**
2. **Political Engine (NN + RL training target)**
3. **Advisor Council Layer (small LLM fine-tuning target)**

## What this revision fixes
- Removes legacy C++/Windows-bound runtime from active code paths.
- Provides a deterministic Python baseline that runs without external ML dependencies.
- Adds a true **20,000 action** hierarchical catalog for RL experiments.
- Separates concerns into `core`, `models`, `rl`, `llm`, and `interfaces` modules.

## Architecture

```text
action_id -> ActionCatalog (hierarchical action)
          -> MayorSimulator
               -> EconomicEngine (TinyMLP)
               -> PoliticalEngine (TinyMLP)
               -> FusionEngine
          -> state + rewards
          -> AdvisorCouncil narrative brief
```

## Action Space (20K)
`ActionCatalog` deterministically builds 20,000 unique actions using:
- 8 domains
- 10 policy families
- 10 variants
- 5 intensity levels
- 5 targeting modes
- 5 durations

## Portability
- Python 3.10+
- No hardcoded machine paths
- No OS-specific APIs
- Dependency-free core runtime

## Next Training Steps
1. Replace `TinyMLP` with PyTorch modules behind a feature flag.
2. Add PPO/SAC training loops for economic and political objectives.
3. Add fusion-head training for uncertainty and tradeoff scoring.
4. Fine-tune a small advisor LLM (LoRA/QLoRA) on state-to-brief corpora.
5. Build a dashboard API and front-end.

## Run
```bash
python -m pytest -q
PYTHONPATH=. python scripts/smoke_run.py
```
