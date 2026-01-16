// tagger/Tagger.hpp
// Module for parsing advisor logs and determining simulation state updates.
#pragma once
#include <string>
#include <nlohmann/json.hpp>

class Tagger {
public:
    Tagger(const std::string& exePath, const std::string& modelPath, int maxTokens = 40);

    // Analyze raw advisor logs and extract any changes to city metrics or events.
    nlohmann::json tagDirtyLogs(const std::string& advisorLogs);
    // Merge the extracted update with the current state to produce final state changes.
    nlohmann::json analyzeSimulationUpdate(const std::string& rawUpdateJson, const std::string& currentState);

private:
    std::string exePath_;
    std::string modelPath_;
    int maxTokens_;

    // Utility to run llama.cpp with a given prompt and capture output.
    std::string runLlama(const std::string& prompt) const;
    // Helper to sanitize text for safe inclusion in a prompt.
    std::string sanitize(const std::string& text) const;
};