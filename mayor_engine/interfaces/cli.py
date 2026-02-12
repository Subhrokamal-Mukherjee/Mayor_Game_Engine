from mayor_engine.core.action_space import ActionCatalog
from mayor_engine.core.simulator import MayorSimulator
from mayor_engine.llm.advisor import AdvisorCouncil


def run_cli() -> None:
    sim = MayorSimulator()
    council = AdvisorCouncil()
    catalog = ActionCatalog(target_count=20_000)
    print("MAYOR Engine v2 CLI scaffold")
    for action_id in (0, 17, 202):
        result = sim.step(catalog.get(action_id))
        print(f"Action #{action_id} | total_reward={result.total_reward:.2f}")
        print(council.render_brief(result.state))
        print("-" * 40)
