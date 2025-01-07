#include "PowerUpLevel1.h"
#include "graphics.h"
#include <iostream>

// Constructor
PowerUpLevel1::PowerUpLevel1(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 1) {}

// Apply effect
void PowerUpLevel1::applyEffect() {
    std::cout << "Level 1 Power-Up effect applied! Weakening MovingEnemies for 10 seconds." << std::endl;

    // Use the centralized weakness logic in PowerUpBase
    startWeakEffect(100.0f); // Updated to 10 seconds
}

// Draw
void PowerUpLevel1::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Brown for Level 1
    br.fill_color[0] = 0.65f; // Brown color
    br.fill_color[1] = 0.16f;
    br.fill_color[2] = 0.16f;

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}
