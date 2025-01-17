﻿#include "MovingEnemy.h"
#include "graphics.h"
#include "GameState.h"
#include <cstdlib>   // For rand()
#include <iostream>  // For debug logs

// Constructor
MovingEnemy::MovingEnemy(GameState* state, int x, int y, bool weak)
    : Enemy(state, x, y, "MovingEnemy"), directionX(0), directionY(0), moveInterval(1500 + rand() % 2500) {
    lastMoveTime = std::chrono::high_resolution_clock::now();

    // Smooth movement variables
    xPos = x * CELL_SIZE + CELL_SIZE / 2;
    yPos = y * CELL_SIZE + CELL_SIZE / 2;
    targetX = xPos;
    targetY = yPos;
    moving = false;
    speed = 0.008f;

    randomizeDirection(); // Set initial random direction

    // Initialize as weak if specified
    if (weak) {
        setWeak(true); // Set to weak state
    }
}

// Initialize behavior
void MovingEnemy::init() {
    randomizeDirection(); // Randomize direction on initialization
}

// Draw the moving enemy
void MovingEnemy::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    if (isInactive) { // Check for inactive state
        br.texture = ASSET_PATH + "objects/MOVING-ENEMY-SPAWNING.png"; // Texture for inactive state
    }
    else if (isWeak()) { // Check for weak state
        br.texture = ASSET_PATH + "objects/MOVING-ENEMY-WEAK.png"; // Texture for weak state
    }
    else {
        br.texture = ASSET_PATH + "objects/MOVING-ENEMY.png"; // Texture for strong state
    }

    br.fill_opacity = 1.0f; // Ensure the texture is fully visible

    // Draw the enemy with the appropriate texture
    graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
}

// Update behavior
void MovingEnemy::update(float dt) {
    // Handle inactive state
    if (isInactive) {
        if (graphics::getGlobalTime() >= inactiveEndTime) {
            // Instead of forcibly resetting isWeak here, just do:
            isInactive = false;
            // If you have an activate() method, call it, but remove any 'isWeak = false' there!
            // activate(); 

            std::cout << "MovingEnemy at (" << gridX << ", " << gridY << ") is now ACTIVE." << std::endl;
        }
        return; // Skip further updates while inactive
    }

    // Handle weak state
    // If the enemy is weak (due to a power-up), skip movement
    if (isWeak()) { // Don't move if weak
        return;
    }

    moveToTarget(dt); // Smooth movement logic

    // The rest of your movement timer logic:
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime).count();

    if (!moving && elapsed >= moveInterval) {
        lastMoveTime = now; // Reset timer
        moveInterval = 1500 + rand() % 2500; // Randomize next interval (1.5–4s)

        randomizeDirection();

        int newGridX = gridX + directionX;
        int newGridY = gridY + directionY;

        if (canMoveTo(newGridX, newGridY)) { // Check for valid move
            gridX = newGridX;
            gridY = newGridY;

            targetX = gridX * CELL_SIZE + CELL_SIZE / 2;
            targetY = gridY * CELL_SIZE + CELL_SIZE / 2;
            moving = true;

            std::cout << "MovingEnemy moved to (" << gridX << ", " << gridY << ") at interval "
                << moveInterval << "ms." << std::endl;
        }
        else {
            randomizeDirection(); // Re-randomize if move is invalid
        }
    }
}

// Move smoothly toward the target
void MovingEnemy::moveToTarget(float dt) {
    if (moving) {
        float dx = targetX - xPos;
        float dy = targetY - yPos;
        float dist = sqrt(dx * dx + dy * dy);
        float step = speed * CELL_SIZE * dt;

        if (dist > step) {
            xPos += step * dx / dist;
            yPos += step * dy / dist;
        }
        else {
            xPos = targetX;
            yPos = targetY;
            moving = false;
        }
    }
}

// Check valid move
bool MovingEnemy::canMoveTo(int x, int y) const {
    // Adjusted to respect grid boundaries
    if (x < 1 || x > PLAYABLE_COLUMNS || y < UI_ROWS_ABOVE || y >= UI_ROWS_ABOVE + PLAYABLE_ROWS) {
        return false; // Out of bounds
    }

    for (const auto& obj : GameState::getInstance()->getGameObjects()) {
        InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
        if (interactive && interactive->isActive() &&
            interactive->getGridX() == x && interactive->getGridY() == y) {
            return false; // Tile is occupied
        }
    }
    return true;
}

// Randomize direction
void MovingEnemy::randomizeDirection() {
    int dir = rand() % 4;
    switch (dir) {
    case 0: directionX = 0; directionY = -1; break; // Up
    case 1: directionX = 0; directionY = 1; break; // Down
    case 2: directionX = -1; directionY = 0; break; // Left
    case 3: directionX = 1; directionY = 0; break; // Right
    }
}

// Set Weak State
void MovingEnemy::setWeak(bool weak) {
    isWeakState = weak;
    if (weak) {
        stopMovement(); // Stop movement
    }
    else {
        startMovement(); // Resume movement
    }
}

// Stop Movement
void MovingEnemy::stopMovement() {
    stopped = true;  // Mark as stopped
    moving = false;  // Prevent further movement
}

// Start Movement
void MovingEnemy::startMovement() {
    stopped = false; // Allow movement again
}

// Collision Handling for MovingEnemy
void MovingEnemy::handleCollision(Player& player) {
    // Ignore collisions if the enemy is inactive
    if (isInactive) {
        return;
    }

    if (isWeak()) {
        graphics::playSound(ASSET_PATH + "sounds/kill.wav", 0.7f, false);
        std::cout << "Weak enemy killed by player!" << std::endl;
        setActive(false); // Deactivate the enemy

        auto* gameState = GameState::getInstance();

        // Notify GameState about the kill
        gameState->addToKillChain(); // Increment kill chain and calculate score
        gameState->enemyKilled = true;
        gameState->enemyKillEndTime = graphics::getGlobalTime() + 1500.0f; // 1.5 seconds from now
    }
    else {
        std::cout << "Player killed by enemy!" << std::endl;
        player.setDead(); // Kill the player if enemy is strong
    }
}
