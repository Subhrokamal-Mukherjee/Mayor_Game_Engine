// TransportAdvisor.h
// Advisor AI specializing in transport and infrastructure analysis.
#pragma once
#include "Advisor.h"
#include <string>

class TransportAdvisor : public Advisor {
public:
    TransportAdvisor(AIInterface& ai);
    std::string getName() const override;
    bool isRelevant(const std::string& directive) const override;

protected:
    std::string getAdvisorIntro() const override;
};
