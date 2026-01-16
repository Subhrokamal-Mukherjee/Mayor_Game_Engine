// QueryRouter.h
// Module for handling structured queries using local simulation data.
#pragma once
#include <string>

class Simulation;  // forward declaration

class QueryRouter {
public:
    // Initialize with a reference to the simulation data source.
    QueryRouter(const Simulation& simulation);

    // Answer the user’s query using the simulation state.
    std::string answerQuery(const std::string& query) const;

private:
    const Simulation& sim_;
};
