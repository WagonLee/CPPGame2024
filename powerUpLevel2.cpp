#include "PowerUpLevel2.h"
#include "graphics.h"
#include <iostream>
#include "config.h"

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
    br.texture = "bin/assets/objects/POWERUP2.png"; // Direct path to the Level 2 texture

    // Use CELL_SIZE for proper alignment
    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    // Draw the power-up with the texture applied
    graphics::drawRect(xPos, yPos, CELL_SIZE * 0.95, CELL_SIZE * 0.95, br);
}
