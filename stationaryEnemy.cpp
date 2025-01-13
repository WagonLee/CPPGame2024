#include "StationaryEnemy.h"
#include "GameState.h"
#include <iostream>
#include "config.h"

// Constructor
StationaryEnemy::StationaryEnemy(GameState* state, int x, int y)
    : Enemy(state, x, y, "StationaryEnemy") {}

// Initialize behavior
void StationaryEnemy::init() {
    // No specific initialization required for stationary enemy
}

// Draw the stationary enemy
void StationaryEnemy::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f; // Remove outlines

    // Ensure texture path is correct and fill opacity is set
    if (isWeak) { // Check weak state correctly
        br.texture = ASSET_PATH + "objects/STATIONARY-ENEMY-WEAK.png"; // Corrected texture path
    }
    else {
        br.texture = ASSET_PATH + "objects/STATIONARY-ENEMY.png"; // Corrected texture path
    }

    br.fill_opacity = 1.0f; // Ensure the texture is fully visible

    // Calculate position on the grid
    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    // Draw the enemy with the appropriate texture
    graphics::drawRect(xPos, yPos - CELL_SIZE/8.1f, CELL_SIZE * 1.3f, CELL_SIZE * 1.3f, br);
}

// Update behavior
void StationaryEnemy::update(float dt) {
    // Stationary enemy does not move, so no update logic required
}
