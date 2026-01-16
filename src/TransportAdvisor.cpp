// TransportAdvisor.cpp
#include "TransportAdvisor.h"
#include <string>
#include <algorithm>

TransportAdvisor::TransportAdvisor(AIInterface& ai)
    : Advisor(ai) {
}

std::string TransportAdvisor::getName() const {
    return "Transport Advisor";
}

std::string TransportAdvisor::getAdvisorIntro() const {
    // Role description and instructions for the transport advisor.
    return
        std::string("You are the City's Transport Advisor, an expert in urban transportation and infrastructure. ") +
        std::string("Your role is to advise the Mayor on public transit, roads, traffic, and infrastructure related to the directive. ") +
        std::string("Provide analysis of transportation impacts and structural considerations, focusing only on those aspects.\n") +
        std::string("Do NOT discuss financial or social issues beyond their effect on transportation. ") +
        std::string("Stay in the role of Transport Advisor (do not roleplay as the Mayor or other characters). ") +
        std::string("Do NOT mention being an AI, and do NOT ask the Mayor additional questions.");
}

bool TransportAdvisor::isRelevant(const std::string& directive) const {
    // Check for keywords indicating a transport/infrastructure-related directive.
    std::string d = directive;
    std::transform(d.begin(), d.end(), d.begin(), ::tolower);
    const std::string keywords[] = {
        "road", "highway", "bridge", "traffic", "transport", "transit",
        "train", "rail", "bus", "subway", "airport", "port", "infrastructure"
    };
    for (const std::string& key : keywords) {
        if (d.find(key) != std::string::npos) {
            return true;
        }
    }
    return false;
}
