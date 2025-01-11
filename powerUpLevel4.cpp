#include "PowerUpLevel4.h"
#include "graphics.h"
#include <iostream>
#include "GameState.h" // Required for collectible spawning
#include "config.h"

// Constructor
PowerUpLevel4::PowerUpLevel4(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 4) {}

// Apply effect
void PowerUpLevel4::applyEffect() {
    std::cout << "Level 4 Power-Up effect applied! Weakening all enemies for 20 seconds and spawning 1 collectible." << std::endl;

    // Use the centralized weakness logic in PowerUpBase
    startWeakEffect(17000.0f); // Updated to 17 seconds

    // Spawn 1 additional collectible immediately
    std::cout << "Spawning 1 additional collectible as part of Level 4 effect." << std::endl;
    state->spawnInteractiveObject<Collectible>();
}

// Draw
void PowerUpLevel4::draw() {

    if (!visible) return;

    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Purple for Level 4
    br.fill_color[0] = 0.0f; // Purple
    br.fill_color[1] = 0.2f;
    br.fill_color[2] = 0.5f;

    // Use CELL_SIZE for proper alignment
    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    // Draw the power-up, slightly smaller than a full cell for aesthetics
    graphics::drawRect(xPos, yPos, CELL_SIZE * 0.8f, CELL_SIZE * 0.8f, br);
}
