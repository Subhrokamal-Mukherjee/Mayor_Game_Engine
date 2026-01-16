// Engine.cpp
#include "Engine.h"
#include "tagger/tagger.hpp"
#include <iostream>
#include <string>

Engine::Engine(AIInterface& ai, UI& ui, Simulation& sim, Tagger& tagger)
    : ai_(ai),
    ui_(ui),
    sim_(sim),
    tagger_(tagger),
    classifier_(),                // not actively used in CLI-driven flow
    queryRouter_(sim),
    contextBuilder_(),
    budgetModule_(),
    finAdvisor_(ai),
    transAdvisor_(ai),
    socialAdvisor_(ai),
    planner_(ai, sim) {
    // Advisors are initialized and ready.
}

void Engine::run() {
    ui_.printWelcome();
    bool running = true;
    while (running) {
        std::string commandType = ui_.promptCategory();
        if (commandType.empty()) continue;
        if (commandType == "help") {
            ui_.printHelp();
            continue;
        }
        if (commandType == "exit" || commandType == "quit") {
            running = false;
            continue;
        }
        if (commandType == "directive") {
            std::string directive = ui_.promptDirective();
            if (directive.empty()) continue;
            std::string response = handleDirective(directive);
            ui_.printAIResponse(response);
        }
        else if (commandType == "query") {
            std::string question = ui_.promptQuery();
            if (question.empty()) continue;
            std::string answer = queryRouter_.answerQuery(question);
            ui_.printAIResponse(answer);
        }
        else if (commandType == "review") {
            std::string report = sim_.getStateSummary();
            report += "Recent Events:\n" + sim_.getRecentEvents(5);
            ui_.printCityStatus(report);
        }
        else if (commandType == "plan") {
            std::string desc = ui_.promptProjectDescription();
            if (desc.empty()) continue;
            nlohmann::json plan = planner_.planAndStore(desc);
            if (plan.is_null()) {
                ui_.printAIResponse("Planning failed. No response.");
            }
            else {
                ui_.printAIResponse(plan.dump(4));
            }
        }
        else if (commandType == "plans") {
            auto plans = planner_.getAllPlans();
            if (plans.empty()) {
                std::cout << "No projects have been planned yet.\n";
            }
            else {
                std::cout << "\n--- Existing Project Plans ---\n";
                for (const auto& plan : plans) {
                    std::cout << plan.dump(4) << "\n\n";
                }
            }
        }
        else {
            std::cout << "Unknown command. Type 'help' for valid commands.\n";
        }
    }
}

std::string Engine::handleDirective(const std::string& directive) {
    // Build context with current state and recent events for the advisors.
    std::string context = contextBuilder_.buildContext(sim_);
    // Collect advice from each relevant advisor.
    std::vector<Advisor*> advisors = { &finAdvisor_, &transAdvisor_, &socialAdvisor_ };
    std::string combinedAdvice;
    std::string combinedLogs;
    for (Advisor* advisor : advisors) {
        if (advisor->isRelevant(directive)) {
            std::string adviceText = advisor->advise(directive, context);
            // Append advisor's advice with a label for output.
            combinedAdvice += advisor->getName() + ": " + adviceText + "\n\n";
            // Append raw advice text to logs (without labels).
            combinedLogs += adviceText + "\n";
        }
    }
    if (combinedAdvice.empty()) {
        // If no advisors responded (unlikely for a valid directive).
        combinedAdvice = "Advisor Council: (No specific advice. The directive might be outside current expertise.)";
        combinedLogs.clear();
    }
    else {
        // Remove any trailing whitespace/newlines from the combined advice.
        while (!combinedAdvice.empty() && isspace(combinedAdvice.back())) {
            combinedAdvice.pop_back();
        }
    }

    // Use Tagger to extract metrics changes and events from the advisors' logs.
    nlohmann::json rawUpdate = tagger_.tagDirtyLogs(combinedLogs);
    // Combine the raw update with current state to get finalized changes.
    std::string stateStr = sim_.getStateSummary();
    nlohmann::json finalUpdate = tagger_.analyzeSimulationUpdate(rawUpdate.dump(), stateStr);

    // Determine the event description for this turn.
    std::string eventDesc = finalUpdate.value("event", "");
    if (eventDesc.empty() || eventDesc == "unknown") {
        // If no event description was identified, use the directive text as a fallback event description.
        eventDesc = directive;
    }
    // Record the event in simulation history.
    sim_.recordEvent(eventDesc);
    // Update simulation metrics with the finalUpdate JSON.
    sim_.updateFromJson(finalUpdate);
    // Log the turn results (saves to CSV/JSON).
    budgetModule_.logTurn(sim_);
    // Advance to the next turn.
    sim_.advanceTurn();

    return combinedAdvice;
}
