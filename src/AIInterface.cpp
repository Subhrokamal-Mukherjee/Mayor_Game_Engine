// AIInterface.cpp
#include "AIInterface.h"
#include <windows.h>
#include <sstream>
#include <iostream>

AIInterface::AIInterface(const std::string& exePath, const std::string& modelPath)
    : exePath_(exePath), modelPath_(modelPath) {
    // Default model arguments (can be adjusted as needed).
    modelArgs_ = {
        "--device CUDA0",
        "--n-gpu-layers 64",
        "--temp 0.8",
        "-no-cnv"
    };
}

std::string AIInterface::buildCommand(const std::string& prompt) const {
    // Build a llama.cpp command with a generic advisor persona.
    std::ostringstream oss;
    oss << "\"" << exePath_ << "\" -m \"" << modelPath_ << "\"";
    for (const std::string& arg : modelArgs_) {
        oss << " " << arg;
    }
    // Default prompt: general city advisor (used by AIInterface::query if needed).
    oss << " -p \"[INST] You are City Advisor, a seasoned expert in city management and strategy. The user is the Mayor. "
        << "Provide strategic guidance and analysis in a formal tone.\n"
        << "NEVER roleplay as the Mayor or reveal you are an AI.\n"
        << "Mayor's query or directive:\n"
        << prompt
        << "\n[/INST]\"";
    return oss.str();
}

std::string AIInterface::invokeLlamaCli(const std::string& command) const {
    // Launch the llama.cpp CLI process with the given command string and capture its output.
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    HANDLE hRead = nullptr, hWrite = nullptr;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        return "Failed to create pipe.";
    }
    PROCESS_INFORMATION pi{};
    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;
    // Launch the process (llama.cpp CLI).
    std::vector<char> cmdBuf(command.begin(), command.end());
    cmdBuf.push_back('\0');
    BOOL ok = CreateProcessA(
        nullptr, cmdBuf.data(),
        nullptr, nullptr, TRUE, CREATE_NO_WINDOW,
        nullptr, nullptr, &si, &pi
    );
    if (!ok) {
        CloseHandle(hWrite);
        CloseHandle(hRead);
        return "Failed to launch llama-cli.";
    }
    // Close write end so we can read output.
    CloseHandle(hWrite);
    std::string result;
    char buffer[4096];
    DWORD bytesRead;
    // Read the process output from the pipe.
    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        result += buffer;
    }
    // Cleanup handles.
    CloseHandle(hRead);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return result;
}

std::string AIInterface::extractAIResponse(const std::string& rawOutput) const {
    // Remove prompt echo and any trailing tokens from the raw output.
    size_t start = rawOutput.find("[/INST]");
    std::string cleaned = (start == std::string::npos) ? rawOutput : rawOutput.substr(start + 7);
    size_t eot = cleaned.find("[end of text]");
    if (eot != std::string::npos) {
        cleaned = cleaned.substr(0, eot);
    }
    return cleaned;
}

std::string AIInterface::query(const std::string& prompt) {
    // Use the default persona to answer a generic prompt or question.
    std::string cmd = buildCommand(prompt);
    std::string raw = invokeLlamaCli(cmd);
    return extractAIResponse(raw);
}
