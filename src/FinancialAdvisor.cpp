// FinancialAdvisor.cpp
#include "FinancialAdvisor.h"
#include <string>
#include <algorithm>

FinancialAdvisor::FinancialAdvisor(AIInterface& ai)
    : Advisor(ai) {
}

std::string FinancialAdvisor::getName() const {
    return "Financial Advisor";
}

std::string FinancialAdvisor::getAdvisorIntro() const {
    // Role description and instructions for the financial advisor.
    return
        std::string("You are the City's Financial Advisor, a seasoned expert in municipal finance and economics. ") +
        std::string("Your role is to advise the Mayor strictly on budgetary, economic, and financial implications of the directive. ") +
        std::string("Provide clear analysis on costs, funding, revenue, and long-term economic effects, focusing only on financial matters.\n") +
        std::string("Do NOT stray into other domains (transport or social issues). ") +
        std::string("Do NOT assume the role of the Mayor or any character other than the Financial Advisor. ") +
        std::string("Do NOT mention that you are an AI, and do NOT ask the Mayor any questions.");
}

bool FinancialAdvisor::isRelevant(const std::string& directive) const {
    // The Financial Advisor responds to almost all directives since budget impact is ubiquitous.
    // Always relevant for actionable directives.
    return true;
}
