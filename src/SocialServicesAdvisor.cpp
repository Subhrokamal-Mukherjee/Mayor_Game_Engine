// SocialServicesAdvisor.cpp
#include "SocialServicesAdvisor.h"
#include <string>
#include <algorithm>

SocialServicesAdvisor::SocialServicesAdvisor(AIInterface& ai)
    : Advisor(ai) {
}

std::string SocialServicesAdvisor::getName() const {
    return "Social Services Advisor";
}

std::string SocialServicesAdvisor::getAdvisorIntro() const {
    // Role description and instructions for the social services advisor.
    return
        std::string("You are the City's Social Services Advisor, an expert in public welfare, healthcare, education, and community well-being. ") +
        std::string("Your role is to advise the Mayor on social implications of the directive, including impacts on public health, education, crime, and citizen satisfaction. ") +
        std::string("Provide analysis focused only on social outcomes and public opinion effects.\n") +
        std::string("Do NOT discuss financial or transport details (those are handled by other advisors). ") +
        std::string("Stay in character as the Social Services Advisor and do not roleplay as anyone else. ") +
        std::string("Do NOT mention that you are an AI, and do NOT ask the Mayor any questions.");
}

bool SocialServicesAdvisor::isRelevant(const std::string& directive) const {
    // Keywords indicating a social/public welfare related directive.
    std::string d = directive;
    std::transform(d.begin(), d.end(), d.begin(), ::tolower);
    const std::string keywords[] = {
        "health", "hospital", "clinic", "disease", "pandemic",
        "education", "school", "teacher", "student", "curriculum",
        "crime", "police", "safety", "violence",
        "poverty", "homeless", "welfare", "community",
        "public opinion", "approval", "satisfaction", "protest"
    };
    for (const std::string& key : keywords) {
        if (d.find(key) != std::string::npos) {
            return true;
        }
    }
    return false;
}
