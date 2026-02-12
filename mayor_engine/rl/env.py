from mayor_engine.core.action_space import ActionCatalog
from mayor_engine.core.simulator import MayorSimulator


class MayorEnv:
    """Dependency-free RL-style environment.

    Uses a discrete action id mapped into a hierarchical 20K action catalog.
    """

    def __init__(self, max_turns: int = 260, action_count: int = 20_000) -> None:
        self.sim = MayorSimulator()
        self.max_turns = max_turns
        self.catalog = ActionCatalog(target_count=action_count)

    @property
    def action_size(self) -> int:
        return len(self.catalog)

    def reset(self) -> tuple[list[float], dict]:
        self.sim.reset()
        return self._obs(), {"action_size": self.action_size}

    def step(self, action_id: int):
        mapped = self.catalog.get(action_id)
        result = self.sim.step(mapped)
        done = result.state.turn >= self.max_turns
        info = {
            "state": result.state.model_dump(),
            "economic_reward": result.economic_reward,
            "political_reward": result.political_reward,
        }
        return self._obs(), result.total_reward, done, info

    def _obs(self) -> list[float]:
        s = self.sim.state
        return [
            s.budget,
            s.gdp_index,
            s.unemployment_rate,
            s.inflation_index,
            s.public_approval,
            s.social_stability,
            s.trust_index,
            s.protest_risk,
        ]
