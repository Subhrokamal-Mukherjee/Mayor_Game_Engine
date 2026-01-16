// SocialServicesAdvisor.h
// Advisor AI specializing in social services and public welfare analysis.
#pragma once
#include "Advisor.h"
#include <string>

class SocialServicesAdvisor : public Advisor {
public:
    SocialServicesAdvisor(AIInterface& ai);
    std::string getName() const override;
    bool isRelevant(const std::string& directive) const override;

protected:
    std::string getAdvisorIntro() const override;
};
