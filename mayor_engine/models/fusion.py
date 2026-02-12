from dataclasses import replace

from mayor_engine.core.schema import CityState


class FusionEngine:
    def apply(self, state: CityState, econ_delta: dict[str, float], pol_delta: dict[str, float]) -> CityState:
        merged = replace(state)
        for key, value in {**econ_delta, **pol_delta}.items():
            if hasattr(merged, key):
                setattr(merged, key, getattr(merged, key) + value)
        merged.public_approval = max(0.0, min(100.0, merged.public_approval))
        merged.social_stability = max(0.0, min(100.0, merged.social_stability))
        merged.trust_index = max(0.0, min(100.0, merged.trust_index))
        merged.protest_risk = max(0.0, min(100.0, merged.protest_risk))
        return merged
