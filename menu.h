#pragma once
#include <vector>
#include <string>
#include "GridRenderer.h" // For Tile structure

class Menu {
private:
    int selectedOption = 0; // Tracks the currently highlighted option
    std::vector<std::vector<Tile>> menuGridState; // Separate grid for menu rendering

    // Chase animation state variables
    float elapsedTime = 0.0f; // Tracks elapsed time for chase animation
    int enemyCol = 0;         // Column position for MOVING-ENEMY
    int playerCol = 2;        // Column position for PLAYER-R

    void updateChaseAnimation(float dt); // Updates the chase animation

public:
    void update(); // Handles navigation and selection
    void draw();   // Renders the menu on the menu grid
    void init();   // Initializes resources like fonts, sounds, and menu grid
};
