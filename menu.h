#pragma once
#include <vector>
#include <string>
#include "GridRenderer.h" // For Tile structure

class Menu {
private:
    int selectedOption = 0; // Tracks the currently highlighted option
    std::vector<std::vector<Tile>> menuGridState; // Separate grid for menu rendering

public:
    void update(); // Handles navigation and selection
    void draw();   // Renders the menu on the menu grid
    void init();   // Initializes resources like fonts, sounds, and menu grid
};
