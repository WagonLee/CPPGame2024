#include "StationaryEnemy.h"
#include "GameState.h"
#include <iostream>

const float CELL_SIZE = 50.0f; // Match grid cell size

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
    br.outline_opacity = 0.0f;

    if (isWeak) {
        br.fill_color[0] = 0.5f; // Light cyan for weak state
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;
    }
    else {
        br.fill_color[0] = 0.0f; // Cyan for strong state
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;
    }

    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
}

// Update behavior
void StationaryEnemy::update(float dt) {
    // Stationary enemy does not move, so no update logic required
}
