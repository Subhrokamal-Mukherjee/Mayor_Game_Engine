// tagger/Tagger.cpp
#include "tagger/Tagger.hpp"
#include <windows.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

Tagger::Tagger(const std::string& exePath, const std::string& modelPath, int maxTokens)
    : exePath_(exePath), modelPath_(modelPath), maxTokens_(maxTokens) {
}

std::string Tagger::sanitize(const std::string& text) const {
    // Replace double quotes with single quotes to avoid breaking JSON formatting in prompts.
    std::string result = text;
    std::replace(result.begin(), result.end(), '"', '\'');
    return result;
}

std::string Tagger::runLlama(const std::string& prompt) const {
    // Build the llama.cpp command with low temperature for deterministic output.
    std::string cmd = "\"";
    cmd += exePath_ + "\" -m \"" + modelPath_ + "\"";
    cmd += " --device CUDA0";
    cmd += " --n-gpu-layers 64";
    cmd += " --threads 4";  // use a few threads (adjust as needed)
    cmd += " --temp 0.2";
    cmd += " --n_predict " + std::to_string(maxTokens_);
    cmd += " -no-cnv";
    cmd += " -p \"" + prompt + "\"";

    // Execute the command and capture output (similar approach as AIInterface).
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    HANDLE hRead = nullptr, hWrite = nullptr;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        return "";
    }
    PROCESS_INFORMATION pi{};
    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;
    std::vector<char> cmdBuf(cmd.begin(), cmd.end());
    cmdBuf.push_back('\0');
    BOOL ok = CreateProcessA(
        nullptr, cmdBuf.data(),
        nullptr, nullptr, TRUE, CREATE_NO_WINDOW,
        nullptr, nullptr, &si, &pi
    );
    if (!ok) {
        CloseHandle(hWrite);
        CloseHandle(hRead);
        return "";
    }
    CloseHandle(hWrite);
    std::string result;
    char buffer[1024];
    DWORD bytesRead;
    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        result += buffer;
    }
    CloseHandle(hRead);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return result;
}

nlohmann::json Tagger::tagDirtyLogs(const std::string& advisorLogs) {
    if (advisorLogs.empty()) {
        // If there's no log text, return an empty JSON.
        return nlohmann::json::object();
    }
    // Sanitize the advisor logs.
    std::string logs = sanitize(advisorLogs);

    // Build the prompt to extract changes from logs.
    std::string prompt = "[INST] You are a log analysis assistant for a city simulation.\n";
    prompt += "Read the advisor statements below and identify any changes to city metrics or notable events.\n";
    prompt += "Provide the output as a JSON object with the keys: \"budget\", \"public_opinion\", \"healthcare\", \"infrastructure\", \"employment\", \"education\", \"crime\", and \"event\".\n";
    prompt += "Use numeric values if provided, or \"unknown\" if a value isn't mentioned. For \"event\", give a short description of what happened.\n";
    prompt += "Advisor Statements:\n" + logs + "\n";
    prompt += "JSON Output:\n[/INST]";

    // Run the LLM on this prompt.
    std::string rawOutput = runLlama(prompt);
    if (rawOutput.empty()) {
        return nlohmann::json::object();
    }

    // Attempt to isolate JSON from the output (in case of extra text).
    size_t start = rawOutput.find('{');
    size_t end = rawOutput.rfind('}');
    if (start == std::string::npos || end == std::string::npos || end < start) {
        // No JSON found, return empty object.
        return nlohmann::json::object();
    }
    std::string jsonStr = rawOutput.substr(start, end - start + 1);

    // Parse JSON (ignore errors if any).
    nlohmann::json parsed;
    try {
        parsed = nlohmann::json::parse(jsonStr);
    }
    catch (...) {
        return nlohmann::json::object();
    }
    return parsed;
}

nlohmann::json Tagger::analyzeSimulationUpdate(const std::string& rawUpdateJson, const std::string& currentState) {
    // Parse the raw update JSON string into a JSON object.
    nlohmann::json update;
    try {
        update = nlohmann::json::parse(rawUpdateJson);
    }
    catch (...) {
        // If parsing fails, assume no updates.
        return nlohmann::json::object();
    }

    // Ensure all keys exist in the update (fill missing with "unknown").
    const char* keys[] = { "budget", "public_opinion", "healthcare", "infrastructure",
                            "employment", "education", "crime", "event" };
    for (const char* key : keys) {
        if (!update.contains(key)) {
            update[key] = "unknown";
        }
    }

    // Sanitize current state string (replace quotes, just in case).
    std::string stateText = sanitize(currentState);
    if (!stateText.empty() && stateText.back() == '\n') {
        stateText.pop_back();
    }

    // Build prompt to merge current state and proposed updates.
    std::string prompt = "[INST] You are a simulation update assistant.\n";
    prompt += "You will be given the current state of the city and a proposed state update from advisors. ";
    prompt += "Merge them into a final updated state in JSON format, following these rules:\n";
    prompt += "- If a proposed value is a number, treat it as the new absolute value for that metric.\n";
    prompt += "- If a proposed value is a relative change (e.g., '+5' or 'decrease by 5%'), apply that change to the current value to get the final value.\n";
    prompt += "- If a proposed value is \"unknown\" or not provided, use the current state's value for that metric.\n";
    prompt += "- Always output all metrics (budget, public_opinion, healthcare, infrastructure, employment, education, crime) and an event description in the JSON.\n";
    prompt += "Current State:\n" + stateText + "\n";
    prompt += "Proposed Update:\n";
    // Add each field from the update JSON in a human-readable form.
    for (const char* key : keys) {
        std::string val;
        if (update[key].is_string()) {
            val = update[key].get<std::string>();
        }
        else if (update[key].is_number()) {
            // Convert number to string.
            val = update[key].dump();
        }
        else {
            val = "unknown";
        }
        prompt += std::string(key) + ": " + val + "\n";
    }
    prompt += "Now output the final updated state as JSON:\n[/INST]";

    // Run the LLM with this prompt.
    std::string rawOutput = runLlama(prompt);
    if (rawOutput.empty()) {
        return nlohmann::json::object();
    }

    // Extract the JSON from the output.
    size_t start = rawOutput.find('{');
    size_t end = rawOutput.rfind('}');
    if (start == std::string::npos || end == std::string::npos || end < start) {
        return nlohmann::json::object();
    }
    std::string jsonStr = rawOutput.substr(start, end - start + 1);
    // Parse final JSON (ignore errors, just return empty if any).
    nlohmann::json finalState;
    try {
        finalState = nlohmann::json::parse(jsonStr);
    }
    catch (...) {
        return nlohmann::json::object();
    }
    return finalState;
}


