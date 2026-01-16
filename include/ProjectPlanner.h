#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class AIInterface;
class Simulation;

class ProjectPlanner {
public:
    ProjectPlanner(AIInterface& ai, Simulation& sim);

    // Given a project description, generate and store a structured project plan
    nlohmann::json planAndStore(const std::string& description);

    // Get all stored project plans
    std::vector<nlohmann::json> getAllPlans() const;

private:
    AIInterface& ai_;
    Simulation& sim_;
    std::vector<nlohmann::json> plans_;  // persistent plan storage

    std::string sanitize(const std::string& input) const;
    std::string stripResponse(const std::string& raw) const;
};
