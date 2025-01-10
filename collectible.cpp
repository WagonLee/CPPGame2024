#include "Collectible.h"
#include "graphics.h"
#include "GameState.h"
#include "Player.h" // Include Player to modify tail
#include <iostream> // For debug output

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

    // Use CELL_SIZE from config.h for consistent alignment
    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    // Draw the collectible, slightly smaller than a full cell for aesthetics
    graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
}


// Update behavior
void Collectible::update(float dt) {}

// Initialize behavior
void Collectible::init() {
    std::cout << "Collectible initialized at: (" << gridX << ", " << gridY << ")\n";
}

// Handle collision
void Collectible::handleCollision(Player& player) {
    std::cout << "Collectible collected at: (" << gridX << ", " << gridY << ")\n";

    // Add a tail segment when collected
    player.addTailSegment(); // NEW: Add tail segment to player

    setActive(false); // Collectible disappears
    GameState::getInstance()->scheduleCollectibleRespawn(); // Schedule respawn
}
