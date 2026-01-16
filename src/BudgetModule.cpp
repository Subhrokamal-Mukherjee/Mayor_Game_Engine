// BudgetModule.cpp
#include "BudgetModule.h"
#include "Simulation.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <direct.h>    // for _mkdir (on Windows)
#include <errno.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void BudgetModule::logTurn(const Simulation& sim) {
    // Ensure the logs directory exists (create if not present).
    if (_mkdir("logs") != 0 && errno != EEXIST) {
        std::cerr << "Warning: Failed to create logs directory.\n";
    }

    // Open CSV file for appending.
    std::ofstream csvFile("logs/simulation_state.csv", std::ios::app);
    if (!csvFile.is_open()) {
        std::cerr << "Error: Could not open logs/simulation_state.csv for writing.\n";
    }
    else {
        // Write header if file is empty (tellp() == 0 at beginning).
        if (csvFile.tellp() == 0) {
            csvFile << "turn,budget,public_opinion,healthcare,infrastructure,employment,education,crime,event\n";
        }
        // Gather current state data.
        int turn = sim.getTurn();
        long long budget = sim.getBudget();
        int opinion = sim.getPublicOpinion();
        int health = sim.getHealthcare();
        int infra = sim.getInfrastructure();
        int employ = sim.getEmployment();
        int education = sim.getEducation();
        int crime = sim.getCrime();
        std::string event = sim.getLastEvent();
        // Escape double quotes in the event description (replace with single quotes).
        std::replace(event.begin(), event.end(), '"', '\'');

        // Write a CSV line (event field quoted to allow commas).
        csvFile << turn << ","
            << budget << ","
            << opinion << ","
            << health << ","
            << infra << ","
            << employ << ","
            << education << ","
            << crime << ","
            << "\"" << event << "\""
            << "\n";
    }
    csvFile.close();

    // Prepare JSON snapshot of current state for quick reference or AI context.
    json stateJson;
    stateJson["turn"] = sim.getTurn();
    stateJson["budget"] = sim.getBudget();
    stateJson["public_opinion"] = sim.getPublicOpinion();
    stateJson["healthcare"] = sim.getHealthcare();
    stateJson["infrastructure"] = sim.getInfrastructure();
    stateJson["employment"] = sim.getEmployment();
    stateJson["education"] = sim.getEducation();
    stateJson["crime"] = sim.getCrime();
    stateJson["event"] = sim.getLastEvent();

    std::ofstream jsonFile("logs/city_state.json");
    if (!jsonFile.is_open()) {
        std::cerr << "Error: Could not open logs/city_state.json for writing.\n";
    }
    else {
        jsonFile << stateJson.dump(4);  // pretty-print JSON with indent of 4 spaces
    }
    jsonFile.close();
}
