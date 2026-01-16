// FinancialAdvisor.h
// Advisor AI specializing in financial and budgetary analysis.
#pragma once
#include "Advisor.h"
#include <string>

class FinancialAdvisor : public Advisor {
public:
    FinancialAdvisor(AIInterface& ai);
    std::string getName() const override;
    bool isRelevant(const std::string& directive) const override;

protected:
    std::string getAdvisorIntro() const override;
};
