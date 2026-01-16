// UI.cpp
#include "UI.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

void UI::printWelcome() const {
    std::cout << "Welcome to the MAYOR city simulation engine!" << std::endl;
}

void UI::printHelp() const {
    std::cout << "\nCOMMAND TYPES:\n"
        << " - directive  -> Issue a new city management directive\n"
        << " - query      -> Ask about a metric or report (e.g., budget, ridership)\n"
        << " - review     -> Show current city status and recent events\n"
        << " - plan       -> Plan a new project or initiative\n"
        << " - exit       -> Save and exit the simulation\n";
}

std::string UI::promptCategory() const {
    std::cout << "\nWhat would you like to do? (Type 'help' for options)\n> ";
    std::string input;
    std::getline(std::cin, input);
    // Trim whitespace and convert to lowercase for consistency.
    input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));
    input.erase(input.find_last_not_of(" \t\n\r\f\v") + 1);
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

std::string UI::promptDirective() const {
    std::cout << "\nEnter your directive:\n> ";
    std::string directive;
    std::getline(std::cin, directive);
    return directive;
}

std::string UI::promptQuery() const {
    std::cout << "\nWhat would you like to know?\n> ";
    std::string question;
    std::getline(std::cin, question);
    return question;
}

std::string UI::promptProjectDescription() const {
    std::cout << "\nDescribe the project you'd like to plan:\n> ";
    std::string project;
    std::getline(std::cin, project);
    return project;
}

void UI::printCityStatus(const std::string& statusReport) const {
    std::cout << "\n---- City Status Report ----\n" << statusReport;
    std::cout.flush();
}

void UI::printAIResponse(const std::string& response) const {
    std::cout << "\nAI Response:\n";
    // Print the response with a brief "typewriter" effect for realism.
    for (char c : response) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    std::cout << std::endl;
}
