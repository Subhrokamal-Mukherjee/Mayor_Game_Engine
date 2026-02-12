from mayor_engine.core.schema import CityState


class AdvisorCouncil:
    def render_brief(self, state: CityState) -> str:
        fiscal_risk = "HIGH" if state.inflation_index > 110 else "MODERATE" if state.inflation_index > 103 else "LOW"
        social_risk = "HIGH" if state.protest_risk > 40 else "MODERATE" if state.protest_risk > 20 else "LOW"
        return (
            "Mayor Council Brief\n"
            f"- Turn: {state.turn}\n"
            f"- Budget: {state.budget:,.0f}\n"
            f"- GDP Index: {state.gdp_index:.2f}\n"
            f"- Unemployment Rate: {state.unemployment_rate:.2f}%\n"
            f"- Public Approval: {state.public_approval:.2f}/100\n"
            f"- Protest Risk: {state.protest_risk:.2f} ({social_risk})\n"
            f"- Fiscal Risk: {fiscal_risk}\n"
            "Recommendation: Use balanced policies; pair growth actions with trust-building interventions."
        )
