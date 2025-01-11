#include "PowerUpLevel3.h"
#include "graphics.h"
#include <iostream>
#include "GameState.h" // MULTI MAYBE?
#include "config.h"

// Constructor
PowerUpLevel3::PowerUpLevel3(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 3) {}

// Apply effect
void PowerUpLevel3::applyEffect() {
    std::cout << "Level 3 Power-Up effect applied! Weakening all enemies for 20 seconds and spawning 1 collectible." << std::endl;

    // Use the centralized weakness logic in PowerUpBase
    startWeakEffect(17000.0f); // Updated to 17 seconds

}

// Draw
void PowerUpLevel3::draw() {

    if (!visible) return;

    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Purple for Level 3
    br.fill_color[0] = 0.5f; // Purple
    br.fill_color[1] = 0.0f;
    br.fill_color[2] = 0.5f;

    // Use CELL_SIZE for proper alignment
    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    // Draw the power-up, slightly smaller than a full cell for aesthetics
    graphics::drawRect(xPos, yPos, CELL_SIZE * 0.8f, CELL_SIZE * 0.8f, br);
}
