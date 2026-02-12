from dataclasses import dataclass
from itertools import product

from mayor_engine.core.schema import MayorAction


@dataclass(frozen=True)
class ActionDimensions:
    domains: tuple[str, ...] = (
        "economy",
        "housing",
        "transport",
        "health",
        "education",
        "safety",
        "energy",
        "environment",
    )
    policy_families: tuple[str, ...] = (
        "tax",
        "subsidy",
        "regulation",
        "capex",
        "staffing",
        "service_change",
        "communications",
        "partnership",
        "zoning",
        "enforcement",
    )
    variants: tuple[str, ...] = tuple(f"v{i:02d}" for i in range(1, 11))
    targets: tuple[str, ...] = (
        "citywide",
        "district_north",
        "district_south",
        "district_east",
        "district_west",
    )
    durations: tuple[int, ...] = (1, 2, 4, 8, 12)


class ActionCatalog:
    """Generates a stable catalog of hierarchical actions.

    The combinatorics exceed 20K actions, so we keep a deterministic first-N subset.
    """

    def __init__(self, target_count: int = 20_000, dims: ActionDimensions | None = None) -> None:
        self._dims = dims or ActionDimensions()
        self._actions = self._build_actions(target_count)

    def _build_actions(self, target_count: int) -> list[MayorAction]:
        actions: list[MayorAction] = []
        combos = product(
            self._dims.domains,
            self._dims.policy_families,
            self._dims.variants,
            range(1, 6),
            self._dims.targets,
            self._dims.durations,
        )
        for domain, family, variant, intensity, target, duration in combos:
            actions.append(
                MayorAction(
                    domain=domain,
                    policy_family=family,
                    policy_variant=variant,
                    intensity=intensity,
                    targeting=target,
                    duration_turns=duration,
                )
            )
            if len(actions) >= target_count:
                break
        return actions

    def __len__(self) -> int:
        return len(self._actions)

    def get(self, action_id: int) -> MayorAction:
        return self._actions[action_id]

    def to_id(self, action: MayorAction) -> int:
        for idx, item in enumerate(self._actions):
            if item == action:
                return idx
        raise ValueError("Action not in catalog")
