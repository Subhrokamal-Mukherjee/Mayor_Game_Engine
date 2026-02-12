from mayor_engine.core.action_space import ActionCatalog
from mayor_engine.core.schema import MayorAction
from mayor_engine.core.simulator import MayorSimulator
from mayor_engine.rl.env import MayorEnv


def test_simulator_step_increments_turn():
    sim = MayorSimulator()
    action = MayorAction(
        domain="economy",
        policy_family="tax",
        policy_variant="vat",
        intensity=2,
        targeting="citywide",
        duration_turns=2,
    )
    result = sim.step(action)
    assert result.state.turn == 1


def test_action_catalog_has_20k_items():
    catalog = ActionCatalog(target_count=20_000)
    assert len(catalog) == 20_000


def test_env_step_returns_expected_shape():
    env = MayorEnv(max_turns=3, action_count=64)
    obs, info = env.reset()
    assert len(obs) == 8
    assert info["action_size"] == 64
    obs2, reward, done, info2 = env.step(2)
    assert len(obs2) == 8
    assert isinstance(reward, float)
    assert isinstance(done, bool)
    assert "state" in info2
