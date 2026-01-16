// QueryRouter.cpp
#include "QueryRouter.h"
#include "Simulation.h"
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>
#include <vector>
#include <sstream>

QueryRouter::QueryRouter(const Simulation& simulation)
    : sim_(simulation) {
}

std::string QueryRouter::answerQuery(const std::string& query) const {
    // Convert query to lowercase for case-insensitive matching.
    std::string q = query;
    std::locale loc;
    for (char& c : q) {
        c = std::tolower(c, loc);
    }

    std::vector<std::string> answers;

    // Check for keywords relating to known simulation data and prepare answers.
    if (q.find("ridership") != std::string::npos || q.find("subway") != std::string::npos || q.find("train") != std::string::npos) {
        // Example: no local data for subway ridership yet.
        answers.emplace_back("I'm sorry, we currently have no data on subway ridership.");
    }
    if (q.find("unemployment") != std::string::npos) {
        // Simulation tracks employment rate (percentage employed).
        int employmentRate = sim_.getEmployment();
        int unemploymentRate = 100 - employmentRate;
        answers.emplace_back("The unemployment rate is approximately " + std::to_string(unemploymentRate) + "%.");
    }
    if (q.find("budget") != std::string::npos) {
        answers.emplace_back("The current budget is $" + std::to_string(sim_.getBudget()) + ".");
    }
    if (q.find("public opinion") != std::string::npos || q.find("approval") != std::string::npos) {
        answers.emplace_back("Public opinion is at " + std::to_string(sim_.getPublicOpinion()) + " out of 100.");
    }
    if (q.find("healthcare") != std::string::npos || q.find("health care") != std::string::npos || q.find("health") != std::string::npos) {
        answers.emplace_back("The healthcare index is " + std::to_string(sim_.getHealthcare()) + " (out of 100).");
    }
    if (q.find("infrastructure") != std::string::npos) {
        answers.emplace_back("The infrastructure index is " + std::to_string(sim_.getInfrastructure()) + " (out of 100).");
    }
    if (q.find("employment") != std::string::npos && q.find("unemployment") == std::string::npos) {
        answers.emplace_back("The employment rate is " + std::to_string(sim_.getEmployment()) + "%.");
    }
    if (q.find("education") != std::string::npos || q.find("school") != std::string::npos) {
        answers.emplace_back("The education index is " + std::to_string(sim_.getEducation()) + " (out of 100).");
    }
    if (q.find("crime") != std::string::npos || q.find("safety") != std::string::npos) {
        answers.emplace_back("The crime index is " + std::to_string(sim_.getCrime()) + " (lower is better).");
    }

    if (!answers.empty()) {
        // If multiple pieces of information, join them with newlines.
        std::ostringstream oss;
        for (size_t i = 0; i < answers.size(); ++i) {
            oss << answers[i];
            if (i < answers.size() - 1) {
                oss << "\n";
            }
        }
        return oss.str();
    }

    // If we didn't find any known metric, check if it's a general status query.
    if ((q.find("overall") != std::string::npos) || (q.find("status") != std::string::npos) ||
        (q.find("city") != std::string::npos && q.find("doing") != std::string::npos)) {
        // Provide a summary of the city state.
        return std::string("City status:\n") + sim_.getStateSummary();
    }

    // Check for location-specific queries which the simulation does not support.
    if (q.find("district") != std::string::npos || q.find("ward") != std::string::npos || q.find("neighborhood") != std::string::npos) {
        return "I'm sorry, detailed district-level data is not currently available.";
    }

    // At this point, the query is not recognized. Optionally, an LLM could parse the query here.
    // For now, respond that the information is not available.
    // (Future enhancement: use an LLM to interpret complex questions into structured queries.)
    return "I'm sorry, I don't have information on that topic.";
}