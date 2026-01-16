// Simulation.cpp
#include "Simulation.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>

using json = nlohmann::json;

Simulation::Simulation()
    : budget_(100000000),  // starting budget ($100,000,000)
    public_opinion_(46), // out of 100
    healthcare_(77),     // out of 100
    infrastructure_(56), // out of 100
    employment_(68),     // percentage of population employed
    education_(62),      // out of 100
    crime_(5),           // crime index (lower is better)
    turn_(0),
    last_event_("") {
}

std::string Simulation::getStateSummary() const {
    std::ostringstream oss;
    oss << "budget: " << budget_ << "\n";
    oss << "public_opinion: " << public_opinion_ << "\n";
    oss << "healthcare: " << healthcare_ << "\n";
    oss << "infrastructure: " << infrastructure_ << "\n";
    oss << "employment: " << employment_ << "\n";
    oss << "education: " << education_ << "\n";
    oss << "crime: " << crime_ << "\n";
    return oss.str();
}

std::string Simulation::getLastEvent() const {
    return last_event_;
}

std::string Simulation::getRecentEvents(int count) const {
    std::ostringstream oss;
    int start = std::max(0, static_cast<int>(event_history_.size()) - count);
    for (int i = start; i < static_cast<int>(event_history_.size()); ++i) {
        oss << "- " << event_history_[i] << "\n";
    }
    return oss.str();
}

void Simulation::recordEvent(const std::string& event) {
    last_event_ = event;
    event_history_.push_back(event);
    if (static_cast<int>(event_history_.size()) > max_events_) {
        // Keep only the last max_events_ events.
        event_history_.erase(event_history_.begin());
    }
}

void Simulation::updateFromJson(const json& update) {
    try {
        if (update.contains("budget") && update["budget"].is_number()) {
            budget_ = update["budget"].get<long long>();
        }
        if (update.contains("public_opinion") && update["public_opinion"].is_number()) {
            public_opinion_ = update["public_opinion"].get<int>();
        }
        if (update.contains("healthcare") && update["healthcare"].is_number()) {
            healthcare_ = update["healthcare"].get<int>();
        }
        if (update.contains("infrastructure") && update["infrastructure"].is_number()) {
            infrastructure_ = update["infrastructure"].get<int>();
        }
        if (update.contains("employment") && update["employment"].is_number()) {
            employment_ = update["employment"].get<int>();
        }
        if (update.contains("education") && update["education"].is_number()) {
            education_ = update["education"].get<int>();
        }
        if (update.contains("crime") && update["crime"].is_number()) {
            crime_ = update["crime"].get<int>();
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to update simulation state: ") + e.what());
    }
}

void Simulation::advanceTurn() {
    ++turn_;
}

// Accessors:
int Simulation::getTurn() const { return turn_; }
long long Simulation::getBudget() const { return budget_; }
int Simulation::getPublicOpinion() const { return public_opinion_; }
int Simulation::getHealthcare() const { return healthcare_; }
int Simulation::getInfrastructure() const { return infrastructure_; }
int Simulation::getEmployment() const { return employment_; }
int Simulation::getEducation() const { return education_; }
int Simulation::getCrime() const { return crime_; }
