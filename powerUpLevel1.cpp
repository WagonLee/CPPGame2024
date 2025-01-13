#include "PowerUpLevel1.h"
#include "graphics.h"
#include <iostream>
#include "config.h"

// Constructor
PowerUpLevel1::PowerUpLevel1(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 1) {}

// Apply effect
void PowerUpLevel1::applyEffect() {
    std::cout << "Level 1 Power-Up effect applied! Weakening MovingEnemies for 10 seconds." << std::endl;

    // Use the centralized weakness logic in PowerUpBase
    startWeakEffect(10000.0f); // Updated to 10 seconds
}

// Draw
void PowerUpLevel1::draw() {
    if (!visible) return;

    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.texture = "bin/assets/objects/POWERUP1.png"; // Direct path to the texture

    // Use CELL_SIZE for proper alignment
    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    // Draw the power-up with the texture applied
    graphics::drawRect(xPos, yPos, CELL_SIZE * 0.95, CELL_SIZE * 0.95, br);
}
