#include "ProjectPlanner.h"
#include "AIInterface.h"
#include "Simulation.h"
#include <algorithm>

ProjectPlanner::ProjectPlanner(AIInterface& ai, Simulation& sim)
    : ai_(ai), sim_(sim) {
}

std::string ProjectPlanner::sanitize(const std::string& input) const {
    std::string clean = input;
    std::replace(clean.begin(), clean.end(), '"', '\'');
    return clean;
}

std::string ProjectPlanner::stripResponse(const std::string& raw) const {
    size_t start = raw.find("[/INST]");
    std::string cleaned = (start != std::string::npos) ? raw.substr(start + 7) : raw;
    size_t end = cleaned.find("[end of text]");
    if (end != std::string::npos) cleaned = cleaned.substr(0, end);
    return cleaned;
}

nlohmann::json ProjectPlanner::planAndStore(const std::string& description) {
    std::string context = sim_.getStateSummary();
    std::string prompt =
        "[INST] You are the City's Project Planning Officer.\n"
        "Given the current city state and a proposed project, generate a structured plan including:\n"
        "- Project name\n- Objective\n- Cost estimate\n- Duration (weeks)\n- Affected sectors\n- Risks\n\n"
        "City State:\n" + sanitize(context) + "\n\n"
        "Project Proposal:\n" + sanitize(description) + "\n[/INST]";

    std::string command = ai_.buildCommand(prompt);
    std::string raw = ai_.invokeLlamaCli(command);
    std::string stripped = stripResponse(raw);

    // Try to extract JSON from the result
    size_t start = stripped.find('{');
    size_t end = stripped.rfind('}');
    if (start == std::string::npos || end == std::string::npos || end < start) return {};

    std::string jsonStr = stripped.substr(start, end - start + 1);
    nlohmann::json parsed;
    try {
        parsed = nlohmann::json::parse(jsonStr);
        plans_.push_back(parsed);
    }
    catch (...) {
        return {};
    }
    return parsed;
}

std::vector<nlohmann::json> ProjectPlanner::getAllPlans() const {
    return plans_;
}
