from mayor_engine.core.schema import CityState, MayorAction
from mayor_engine.models.nn import TinyMLP


class PoliticalEngine:
    def __init__(self, seed: int = 11) -> None:
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
            "public_approval": out[0] * 2.0,
            "social_stability": out[1] * 1.6,
            "trust_index": out[2] * 1.5,
            "protest_risk": out[3] * 1.5,
        }
        reward = (state.public_approval * 0.8) + (state.social_stability * 0.7) - (state.protest_risk * 1.3)
        return delta, float(reward)
