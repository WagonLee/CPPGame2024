#include "PowerUpLevel2.h"
#include "graphics.h"
#include <iostream>

// Constructor
PowerUpLevel2::PowerUpLevel2(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 2) {}

// Apply effect
void PowerUpLevel2::applyEffect() {
    std::cout << "Level 2 Power-Up effect applied! Weakening Moving and Stationary Enemies for 15 seconds." << std::endl;

    // Use the centralized weakness logic in PowerUpBase
    startWeakEffect(13500.0f); // Updated to 13.5 seconds
}

// Draw
void PowerUpLevel2::draw() {

    if (!visible) return;

    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Dark Green for Level 2
    br.fill_color[0] = 0.0f;   // Dark Green
    br.fill_color[1] = 0.39f;
    br.fill_color[2] = 0.0f;

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}
