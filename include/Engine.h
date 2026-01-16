// Engine.h
// Central engine orchestrating the AI modules, UI, simulation, and command handling.
#pragma once
#include <vector>
#include <string>
#include "PromptClassifier.h"
#include "QueryRouter.h"
#include "TurnContextBuilder.h"
#include "BudgetModule.h"
#include "FinancialAdvisor.h"
#include "TransportAdvisor.h"
#include "SocialServicesAdvisor.h"
#include "ProjectPlanner.h"
#include "AIInterface.h"
#include "UI.h"
#include "Simulation.h"
#include "tagger/Tagger.hpp"

class Engine {
public:
    Engine(AIInterface& ai, UI& ui, Simulation& sim, Tagger& tagger);
    // Run the main simulation loop until exit.
    void run();

private:
    AIInterface& ai_;
    UI& ui_;
    Simulation& sim_;
    Tagger& tagger_;

    // Modular components for handling various tasks.
    PromptClassifier classifier_;        // (Currently not used in CLI mode)
    QueryRouter queryRouter_;
    TurnContextBuilder contextBuilder_;
    BudgetModule budgetModule_;
    FinancialAdvisor finAdvisor_;
    TransportAdvisor transAdvisor_;
    SocialServicesAdvisor socialAdvisor_;
    ProjectPlanner planner_;

    // Helper to process a directive through advisors and update simulation.
    std::string handleDirective(const std::string& directive);
};
