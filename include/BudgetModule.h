// BudgetModule.h
// Module for reading/writing simulation state to CSV and JSON with safe formatting.
#pragma once
#include <string>

class Simulation;  // forward declaration

class BudgetModule {
public:
    // Save the current simulation state to CSV and JSON files.
    // Appends a line to the CSV and overwrites the JSON snapshot.
    void logTurn(const Simulation& sim);
};
