from dataclasses import asdict, dataclass


@dataclass
class CityState:
    budget: float = 100_000_000
    gdp_index: float = 100.0
    unemployment_rate: float = 6.0
    inflation_index: float = 100.0
    public_approval: float = 50.0
    social_stability: float = 50.0
    trust_index: float = 50.0
    protest_risk: float = 10.0
    turn: int = 0

    def model_dump(self) -> dict:
        return asdict(self)


@dataclass
class MayorAction:
    domain: str
    policy_family: str
    policy_variant: str
    intensity: int = 1
    targeting: str = "citywide"
    duration_turns: int = 1

    def __post_init__(self) -> None:
        if not (1 <= self.intensity <= 5):
            raise ValueError("intensity must be between 1 and 5")
        if not (1 <= self.duration_turns <= 52):
            raise ValueError("duration_turns must be between 1 and 52")
