// NarrativeGenerator.h
// Main narrative engine that coordinates classification, advisors, and simulation updates per turn.
#pragma once
#include <string>
#include <vector>
#include "PromptClassifier.h"

class QueryRouter;
class TurnContextBuilder;
class BudgetModule;
class Simulation;
class Tagger;
class Advisor;

class NarrativeGenerator {
public:
    NarrativeGenerator(PromptClassifier& classifier,
        QueryRouter& queryRouter,
        TurnContextBuilder& contextBuilder,
        BudgetModule& budgetModule,
        Simulation& simulation,
        Tagger& tagger,
        const std::vector<Advisor*>& advisors);

    // Process a single user input (mayoral input) and produce the AI response.
    std::string processInput(const std::string& input);

private:
    PromptClassifier& classifier_;
    QueryRouter& queryRouter_;
    TurnContextBuilder& contextBuilder_;
    BudgetModule& budgetModule_;
    Simulation& sim_;
    Tagger& tagger_;
    std::vector<Advisor*> advisors_;
};
