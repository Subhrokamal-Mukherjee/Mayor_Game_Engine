// Simulation.h
// Holds the city simulation state (metrics and event log management).
#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class Simulation {
public:
    Simulation();

    // Summary of current state (each metric on a new line).
    std::string getStateSummary() const;
    // Get the most recent event description.
    std::string getLastEvent() const;
    // Get a formatted list of the last `count` events (one per line).
    std::string getRecentEvents(int count = 5) const;
    // Record a new event in the history.
    void recordEvent(const std::string& event);
    // Update simulation state from a JSON object (keys matching state fields).
    void updateFromJson(const nlohmann::json& update);
    // Advance to the next turn (increment turn counter).
    void advanceTurn();

    // Accessors for state fields.
    int getTurn() const;
    long long getBudget() const;
    int getPublicOpinion() const;
    int getHealthcare() const;
    int getInfrastructure() const;
    int getEmployment() const;
    int getEducation() const;
    int getCrime() const;

private:
    // City state metrics.
    long long budget_;
    int public_opinion_;
    int healthcare_;
    int infrastructure_;
    int employment_;
    int education_;
    int crime_;
    // Turn counter.
    int turn_;
    // Event log history.
    std::vector<std::string> event_history_;
    std::string last_event_;
    const int max_events_ = 10; // max events to retain in history.
};
