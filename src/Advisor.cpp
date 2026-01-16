// Advisor.cpp
#include "Advisor.h"
#include "AIInterface.h"
#include <algorithm>
#include <string>
#include <thread>  // for hardware_concurrency

Advisor::Advisor(AIInterface& ai)
    : ai_(ai) {
}

std::string Advisor::sanitizeForPrompt(const std::string& text) const {
    std::string result = text;
    std::replace(result.begin(), result.end(), '"', '\'');  // replace all double quotes with single quotes
    return result;
}

std::string Advisor::stripResponse(const std::string& rawOutput) const {
    // Remove everything up to and including the [/INST] tag (which marks end of prompt).
    std::string cleaned = rawOutput;
    size_t instPos = cleaned.find("[/INST]");
    if (instPos != std::string::npos) {
        cleaned = cleaned.substr(instPos + 7);
    }
    // Remove any special end-of-text token if present.
    size_t endTokenPos = cleaned.find("[end of text]");
    if (endTokenPos != std::string::npos) {
        cleaned = cleaned.substr(0, endTokenPos);
    }
    // Trim leading/trailing whitespace and newlines.
    const char* whitespace = " \t\n\r\f\v";
    cleaned.erase(0, cleaned.find_first_not_of(whitespace));
    cleaned.erase(cleaned.find_last_not_of(whitespace) + 1);
    return cleaned;
}

std::string Advisor::advise(const std::string& directive, const std::string& context) {
    // Compose the full prompt for the advisor.
    std::string promptIntro = getAdvisorIntro();
    std::string prompt = "[INST] " + promptIntro + "\n";
    if (!context.empty()) {
        prompt += context;
        if (prompt.back() != '\n') {
            prompt += "\n";
        }
    }
    // Append the Mayor's directive (sanitized).
    prompt += "Mayor's Directive: " + sanitizeForPrompt(directive) + "\n[/INST]";

    // Build the llama.cpp CLI command for this advisor's prompt.
    std::string cmd = "\"";
    cmd += ai_.getExePath();
    cmd += "\" -m \"" + ai_.getModelPath() + "\"";
    cmd += " --device CUDA0";
    cmd += " --n-gpu-layers 64";
    cmd += " --threads " + std::to_string(std::thread::hardware_concurrency());
    cmd += " --temp 0.7";
    cmd += " --n_predict 256";
    cmd += " -no-cnv";
    cmd += " -p \"" + prompt + "\"";

    // Invoke the LLM and capture raw output.
    std::string rawOutput = ai_.invokeLlamaCli(cmd);
    // Strip out any prompt artifacts and return the clean advisor message.
    return stripResponse(rawOutput);
}
