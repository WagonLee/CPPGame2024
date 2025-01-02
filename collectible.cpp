#include "Collectible.h"
#include "graphics.h"

// Constructor
Collectible::Collectible(GameState* state, int x, int y)
    : InteractiveObject(state, x, y, "Collectible") {}

// Draw the collectible (yellow square)
void Collectible::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.fill_color[0] = 1.0f;
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 0.0f;

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}

// Update logic for collectible
void Collectible::update(float dt) {
    // Placeholder for collectible-specific behavior
}

// Initialize collectible
void Collectible::init() {
    // Placeholder for initialization logic
}

// Handle collision
void Collectible::handleCollision() {
    // Placeholder for collision behavior (e.g., removal)
}
