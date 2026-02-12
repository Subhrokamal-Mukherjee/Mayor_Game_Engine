from dataclasses import dataclass

from mayor_engine.core.schema import CityState, MayorAction
from mayor_engine.models.economic import EconomicEngine
from mayor_engine.models.fusion import FusionEngine
from mayor_engine.models.political import PoliticalEngine


@dataclass
class StepResult:
    state: CityState
    economic_reward: float
    political_reward: float
    total_reward: float


class MayorSimulator:
    def __init__(self) -> None:
        self.state = CityState()
        self.economic_engine = EconomicEngine()
        self.political_engine = PoliticalEngine()
        self.fusion_engine = FusionEngine()

    def reset(self) -> CityState:
        self.state = CityState()
        return self.state

    def step(self, action: MayorAction) -> StepResult:
        econ_delta, econ_reward = self.economic_engine.forward(self.state, action)
        pol_delta, pol_reward = self.political_engine.forward(self.state, action)
        self.state = self.fusion_engine.apply(self.state, econ_delta, pol_delta)
        self.state.turn += 1
        total_reward = (econ_reward * 0.55) + (pol_reward * 0.45)
        return StepResult(
            state=self.state,
            economic_reward=econ_reward,
            political_reward=pol_reward,
            total_reward=total_reward,
        )
