// Advisor.h
// Base class for advisor AI modules (financial, transport, social, etc.).
#pragma once
#include <string>

class AIInterface;  // forward declaration

class Advisor {
public:
    Advisor(AIInterface& ai);
    virtual ~Advisor() = default;

    // Generate advisory text for the given directive and context.
    std::string advise(const std::string& directive, const std::string& context);

    // Get the advisor's name (for labeling outputs).
    virtual std::string getName() const = 0;

    // Determine if this advisor is relevant for the given directive.
    virtual bool isRelevant(const std::string& directive) const = 0;

protected:
    // Domain-specific prompt introduction for the advisor (persona and guidelines).
    virtual std::string getAdvisorIntro() const = 0;

    AIInterface& ai_;

private:
    // Replace double quotes with single quotes in a text (to avoid breaking prompt formatting).
    std::string sanitizeForPrompt(const std::string& text) const;
    // Strip the raw LLM output to isolate the advisor's response (remove prompt echoes or tokens).
    std::string stripResponse(const std::string& rawOutput) const;
};
