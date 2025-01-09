#pragma once
#include <vector>
#include <string>

class Menu {
private:
    std::vector<std::string> options = { "PLAY", "HIGH SCORES", "TUTORIAL", "EXIT" };
    int selectedOption = 0; // Tracks the currently highlighted option

public:
    void update(); // Handles navigation and selection
    void draw();   // Renders the menu
    void init();   // Initializes resources like fonts
};
