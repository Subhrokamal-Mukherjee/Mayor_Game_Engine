from mayor_engine.core.schema import CityState, MayorAction
from mayor_engine.models.nn import TinyMLP


class EconomicEngine:
    def __init__(self, seed: int = 7) -> None:
        self.net = TinyMLP(in_dim=10, hidden_dim=16, out_dim=4, seed=seed)

    def _features(self, state: CityState, action: MayorAction) -> list[float]:
        return [
            state.budget / 1e8,
            state.gdp_index / 100,
            state.unemployment_rate / 100,
            state.inflation_index / 100,
            state.public_approval / 100,
            state.social_stability / 100,
            state.trust_index / 100,
            state.protest_risk / 100,
            action.intensity / 5,
            action.duration_turns / 52,
        ]

    def forward(self, state: CityState, action: MayorAction) -> tuple[dict[str, float], float]:
        out = self.net.forward(self._features(state, action))
        delta = {
            "budget": out[0] * 50_000,
            "gdp_index": out[1] * 0.8,
            "unemployment_rate": out[2] * 0.4,
            "inflation_index": out[3] * 0.3,
        }
        reward = (state.gdp_index * 0.6) - (state.unemployment_rate * 2.0) - (state.inflation_index * 0.2)
        return delta, float(reward)
