// PromptClassifier.cpp
#include "PromptClassifier.h"
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

PromptClassifier::Category PromptClassifier::classify(const std::string& input) const {
    // Trim leading/trailing whitespace and lowercase the input for analysis.
    std::string text = input;
    text.erase(0, text.find_first_not_of(" \t\n\r\f\v"));
    text.erase(text.find_last_not_of(" \t\n\r\f\v") + 1);
    if (text.empty()) {
        return Category::OTHER;
    }
    // Convert to lowercase for case-insensitive matching.
    std::string lower;
    lower.reserve(text.size());
    std::locale loc;
    for (char c : text) {
        lower.push_back(std::tolower(c, loc));
    }

    // Check for greeting keywords at start.
    const std::string greetings[] = {
        "hello", "hi", "hey", "good morning", "good afternoon", "good evening", "greetings"
    };
    for (const std::string& greet : greetings) {
        if (lower.rfind(greet, 0) == 0) { // if input starts with a greeting
            return Category::GREETING;
        }
    }
    // Also treat "thanks" or "thank you" as a greeting/acknowledgment.
    if (lower.find("thank") != std::string::npos) {
        return Category::GREETING;
    }

    // Identify structured queries (questions asking for data).
    bool isQuestion = (lower.find('?') != std::string::npos);
    // If the user is asking for advice with "should we...?" treat as a directive rather than factual query.
    if (lower.rfind("should i", 0) == 0 || lower.rfind("should we", 0) == 0 ||
        lower.rfind("could we", 0) == 0 || lower.rfind("can we", 0) == 0 ||
        lower.rfind("what if we", 0) == 0 || lower.rfind("what should we", 0) == 0 ||
        lower.rfind("how can we", 0) == 0 || lower.rfind("what can we", 0) == 0) {
        return Category::DIRECTIVE;
    }
    // If it starts with typical question words, consider it a query.
    const std::string queryWords[] = { "what", "how", "when", "where", "who" };
    for (const std::string& qw : queryWords) {
        if (lower.rfind(qw + " ", 0) == 0 || lower.rfind(qw + "'", 0) == 0) {
            isQuestion = true;
            break;
        }
    }
    if (lower.rfind("tell me", 0) == 0 || lower.rfind("show me", 0) == 0 || lower.rfind("give me", 0) == 0) {
        isQuestion = true;
    }

    if (isQuestion) {
        return Category::QUERY;
    }

    // Default: anything else is treated as a directive (an instruction/decision).
    return Category::DIRECTIVE;
}
