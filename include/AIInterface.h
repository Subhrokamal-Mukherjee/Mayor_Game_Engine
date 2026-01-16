// AIInterface.h
// Interface to the llama.cpp LLM backend (GPU-accelerated).
#pragma once
#include <string>
#include <vector>

class AIInterface {
public:
    AIInterface(const std::string& exePath, const std::string& modelPath);

    // Execute a query prompt with the LLM (using default prompt style for a general advisor).
    std::string query(const std::string& prompt);
    // Build a default query command (used by query() for general queries).
    std::string buildCommand(const std::string& prompt) const;

    // Invoke llama.cpp with a fully-constructed command.
    std::string invokeLlamaCli(const std::string& command) const;

    // Accessors for model and executable paths (used by other modules to build commands).
    const std::string& getExePath() const { return exePath_; }
    const std::string& getModelPath() const { return modelPath_; }

private:
    std::string exePath_;
    std::string modelPath_;
    std::vector<std::string> modelArgs_;

  
    // Extract the assistant's response from raw LLM output (remove prompt echo).
    std::string extractAIResponse(const std::string& rawOutput) const;
};
