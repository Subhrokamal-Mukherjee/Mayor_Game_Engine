// TurnContextBuilder.h
// Module to build context summaries of recent turns/events for AI prompts.
#pragma once
#include <string>

class Simulation;  // forward declaration

class TurnContextBuilder {
public:
    TurnContextBuilder(int maxEvents = 5);

    // Build a context string containing the current state and recent events.
    std::string buildContext(const Simulation& sim) const;

private:
    int maxEvents_;
};
