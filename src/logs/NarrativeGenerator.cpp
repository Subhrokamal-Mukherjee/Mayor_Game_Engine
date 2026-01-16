// NarrativeGenerator.cpp
#include "NarrativeGenerator.h"
#include "QueryRouter.h"
#include "TurnContextBuilder.h"
#include "BudgetModule.h"
#include "Simulation.h"
#include "PromptClassifier.h"
#include "Advisor.h"
#include "tagger/Tagger.hpp"  // Tagger for log extraction and simulation update
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <string>

using json = nlohmann::json;

NarrativeGenerator::NarrativeGenerator(PromptClassifier& classifier,
    QueryRouter& queryRouter,
    TurnContextBuilder& contextBuilder,
    BudgetModule& budgetModule,
    Simulation& simulation,
    Tagger& tagger,
    const std::vector<Advisor*>& advisors)
    : classifier_(classifier),
    queryRouter_(queryRouter),
    contextBuilder_(contextBuilder),
    budgetModule_(budgetModule),
    sim_(simulation),
    tagger_(tagger),
    advisors_(advisors) {
}

std::string NarrativeGenerator::processInput(const std::string& input) {
    // Determine the type of the input (directive, query, greeting, other)
    PromptClassifier::Category category = classifier_.classify(input);

    if (category == PromptClassifier::Category::GREETING) {
        // Simple greeting response
        return std::string("Hello, Mayor! How can I assist you today?");
    }

    if (category == PromptClassifier::Category::QUERY) {
        // Handle informational query using local data
        std::string answer = queryRouter_.answerQuery(input);
        return answer;
    }

    if (category == PromptClassifier::Category::OTHER) {
        // Handle other/unrecognized input gracefully
        return std::string("I am here to assist with city management. Please issue a directive or ask a city-related question.");
    }

    // If we reach here, category is DIRECTIVE
    // Build context from recent turns for advisors
    std::string context = contextBuilder_.buildContext(sim_);

    // Collect advice from each relevant advisor
    std::string combinedAdvice;       // text to show the user (with labels)
    std::string combinedLogs;         // text without labels for internal simulation logging
    for (Advisor* advisor : advisors_) {
        if (advisor->isRelevant(input)) {
            std::string adviceText = advisor->advise(input, context);
            // Label the advice for user output
            combinedAdvice += advisor->getName() + ": " + adviceText + "\n\n";
            // Combine raw advice text for log parsing (no labels)
            combinedLogs += adviceText + "\n";
        }
    }
    if (combinedAdvice.empty()) {
        // If no advisors responded (should not happen in typical cases), fall back to a generic acknowledgement
        combinedAdvice = "Advisor Council: (No specific advice. The directive may be outside current expertise.)\n";
        combinedLogs = "";  // nothing to parse for simulation
    }

    // Use the Tagger module to extract raw logs (metrics/event) from combined advisor outputs
    json rawLogsJson = tagger_.tagDirtyLogs(combinedLogs);
    // Clean and structure the simulation update JSON using current city state for reference
    std::string stateSummary = sim_.getStateSummary();
    json simUpdateJson = tagger_.analyzeSimulationUpdate(rawLogsJson.dump(), stateSummary);
    // Extract event description (if any)
    std::string eventSummary = simUpdateJson.value("event", "");
    if (eventSummary.empty() || eventSummary == "unknown") {
        // If the AI did not identify an event description, use the directive itself as event
        eventSummary = input;
    }
    // Record the event in simulation history
    sim_.recordEvent(eventSummary);
    // Update the simulation state from the structured JSON
    sim_.updateFromJson(simUpdateJson);
    // Log the updated state to CSV/JSON files
    budgetModule_.logTurn(sim_);
    // Advance the simulation turn counter
    sim_.advanceTurn();

    // Trim any trailing whitespace/newlines from combinedAdvice before returning
    if (!combinedAdvice.empty()) {
        // Remove trailing newline and spaces
        while (!combinedAdvice.empty() && (combinedAdvice.back() == '\n' || isspace(combinedAdvice.back()))) {
            combinedAdvice.pop_back();
        }
    }
    return combinedAdvice;
}
