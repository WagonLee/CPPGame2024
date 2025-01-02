#include "Collectible.h"
#include "graphics.h"
#include "GameState.h"
#include <iostream>

// Constructor
Collectible::Collectible(GameState* state, int x, int y)
    : InteractiveObject(state, x, y, "Collectible") {}

// Draw the collectible
void Collectible::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.fill_color[0] = 1.0f; // Yellow
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 0.0f;

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}

// Update behavior
void Collectible::update(float dt) {}

// Initialize behavior (Definition added to fix linker error)
void Collectible::init() {
    std::cout << "Collectible initialized at: (" << gridX << ", " << gridY << ")\n";
}

// Handle collision
void Collectible::handleCollision(Player& player) {
    std::cout << "Collectible collected at: (" << gridX << ", " << gridY << ")\n";
    setActive(false); // Collectible disappears, no respawn
}
