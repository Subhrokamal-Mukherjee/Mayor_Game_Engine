// main.cpp
#include "AIInterface.h"
#include "UI.h"
#include "Simulation.h"
#include "Engine.h"
#include "tagger/Tagger.hpp"
#include <iostream>
#include <string>

int main() {
    // Paths to the llama.cpp CLI and the model file (adjust as needed for your setup).
    const std::string exePath = R"(C:\Users\MAYOR\Desktop\MAYOR_game_engine\external\llama.cpp\build\bin\Release\llama-cli.exe)";
    const std::string modelPath = R"(C:\Users\MAYOR\Desktop\MAYOR_game_engine\external\llama.cpp\models\mistral-7b-instruct-v0.2.Q5_K_M.gguf)";
    // We use the same model for the Tagger; in a real setup, a smaller/faster model could be used for parsing.
    const std::string lowerModelPath = modelPath;

    try {
        // Initialize AI interface, tagger, UI, and simulation.
        AIInterface ai(exePath, modelPath);
        Tagger tagger(exePath, lowerModelPath, 40);
        UI ui;
        Simulation sim;
        // Create the main engine with all components.
        Engine engine(ai, ui, sim, tagger);

        // Run the simulation.
        engine.run();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
