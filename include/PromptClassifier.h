// PromptClassifier.h
// Module for classifying user input intent (directive, query, greeting, other).
#pragma once
#include <string>

class PromptClassifier {
public:
    // Possible categories of user input.
    enum class Category { DIRECTIVE, QUERY, GREETING, OTHER };

    // Classify the given input string into an intent category.
    Category classify(const std::string& input) const;
};
