#include "StationaryEnemy.h"
#include "GameState.h"
#include <iostream>
#include "config.h"

// Constructor
StationaryEnemy::StationaryEnemy(GameState* state, int x, int y)
    : Enemy(state, x, y, "StationaryEnemy") {
    setInactive(4.0f); // Spawn in an inactive state for 4 seconds
}

// Initialize behavior
void StationaryEnemy::init() {
    // No specific initialization required for stationary enemy
}

// Draw the stationary enemy
void StationaryEnemy::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f; // Remove outlines

    // Check state and assign appropriate texture
    if (isInactive) {
        br.texture = ASSET_PATH + "objects/STATIONARY-ENEMY-SPAWNING.png"; // Inactive state texture
    }
    else if (isWeak) {
        br.texture = ASSET_PATH + "objects/STATIONARY-ENEMY-WEAK.png"; // Weak state texture
    }
    else {
        br.texture = ASSET_PATH + "objects/STATIONARY-ENEMY.png"; // Active state texture
    }

    br.fill_opacity = 1.0f; // Ensure the texture is fully visible

    // Calculate position on the grid
    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    // Draw the enemy with the appropriate texture
    graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
}

// Update behavior
void StationaryEnemy::update(float dt) {
    // Handle inactive state
    if (isInactive) {
        if (graphics::getGlobalTime() >= inactiveEndTime) {
            activate(); // Transition to active state
            std::cout << "StationaryEnemy at (" << gridX << ", " << gridY << ") is now ACTIVE." << std::endl;
        }
        return; // Skip further updates while inactive
    }

    // No active behavior for stationary enemy beyond handling inactive state
}
