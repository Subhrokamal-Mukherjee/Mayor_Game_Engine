// TurnContextBuilder.cpp
#include "TurnContextBuilder.h"
#include "Simulation.h"
#include <algorithm>
#include <string>

TurnContextBuilder::TurnContextBuilder(int maxEvents)
    : maxEvents_(maxEvents) {
}

std::string TurnContextBuilder::buildContext(const Simulation& sim) const {
    std::string context;
    // Include current state summary.
    context += "City state:\n" + sim.getStateSummary();
    // Include recent events (up to maxEvents_).
    std::string recent = sim.getRecentEvents(maxEvents_);
    if (!recent.empty()) {
        context += "\nRecent Events:\n";
        // Replace any double quotes with single quotes to avoid prompt issues.
        std::replace(recent.begin(), recent.end(), '"', '\'');
        context += recent;
        // Ensure the context string ends with a newline.
        if (!context.empty() && context.back() != '\n') {
            context.push_back('\n');
        }
    }
    return context;
}
