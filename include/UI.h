// UI.h
// Console-based user interface for the simulation.
#pragma once
#include <string>

class UI {
public:
    void printWelcome() const;
    void printHelp() const;
    std::string promptCategory() const;
    std::string promptDirective() const;
    std::string promptQuery() const;
    std::string promptProjectDescription() const;
    void printCityStatus(const std::string& statusReport) const;
    void printAIResponse(const std::string& response) const;
};
