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

    br.outline_opacity = 0.0f; // No outlines
    br.texture = ASSET_PATH + "objects/COLLECTIBLE.png"; // Use the correct texture path
    br.fill_opacity = 1.0f;   // Ensure texture is visible

    // Use CELL_SIZE from config.h for consistent alignment
    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    // Draw the collectible, slightly smaller than a full cell for aesthetics
    graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
}

// Update behavior
void Collectible::update(float dt) {
    if (isLingering && graphics::getGlobalTime() >= lingeringEndTime) {
        setActive(false); // Deactivate collectible after lingering
        isLingering = false; // Reset lingering state
    }

    //InteractiveObject::update(dt); 
}


// Initialize behavior
void Collectible::init() {
    std::cout << "Collectible initialized at: (" << gridX << ", " << gridY << ")\n";
}

void Collectible::handleCollision(Player& player) {
    std::cout << "Collectible collected at: (" << gridX << ", " << gridY << ")\n";

    // Play sound effect for collectible pickup
    graphics::playSound(ASSET_PATH + "sounds/collect.wav", 1.0f, false);

    // Add a tail segment when collected
    player.addTailSegment(); // Add tail segment to player

    setActive(false); // Collectible disappears
    GameState::getInstance()->scheduleCollectibleRespawn(); // Schedule respawn
}


void Collectible::startLingering(float duration) {
    isLingering = true;
    lingeringEndTime = graphics::getGlobalTime() + duration;
}

bool Collectible::getIsLingering() const {
    return isLingering;
}
