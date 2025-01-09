#include "MovingEnemy.h"
#include "graphics.h"
#include "GameState.h"
#include <cstdlib>   // For rand()
#include <iostream>  // For debug logs

const float CELL_SIZE = 50.0f; // Match grid cell size

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
    speed = 0.002f; // Match the player's speed

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

    if (isWeak()) { // Add parentheses to call it as a function
        br.fill_color[0] = 0.5f; // Light red for weak state
        br.fill_color[1] = 0.0f;
        br.fill_color[2] = 0.0f;
    }
    else {
        br.fill_color[0] = 1.0f; // Original red for strong enemy
        br.fill_color[1] = 0.0f;
        br.fill_color[2] = 0.0f;
    }

    graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
}

// Update behavior
void MovingEnemy::update(float dt) {
    if (isWeak()) { // Don't move if weak
        return;
    }

    moveToTarget(dt); // Smooth movement logic

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
            randomizeDirection();
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
    if (x < 0 || x >= 12 || y < 0 || y >= 12) {
        return false;
    }
    for (const auto& obj : GameState::getInstance()->getGameObjects()) {
        InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
        if (interactive && interactive->isActive() &&
            interactive->getGridX() == x && interactive->getGridY() == y) {
            return false;
        }
    }
    return true;
}

// Randomize direction
void MovingEnemy::randomizeDirection() {
    int dir = rand() % 4;
    switch (dir) {
    case 0: directionX = 0; directionY = -1; break;
    case 1: directionX = 0; directionY = 1; break;
    case 2: directionX = -1; directionY = 0; break;
    case 3: directionX = 1; directionY = 0; break;
    }
}

// Set Weak State
void MovingEnemy::setWeak(bool weak) {
    isWeakState = weak;
    if (weak) {
        stopMovement(); // Stop movement
        std::cout << "Enemy set to WEAK! Color should change." << std::endl;
    }
    else {
        startMovement(); // Resume movement
        std::cout << "Enemy restored to NORMAL! Color should change back." << std::endl;
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

// Collision Handling
void MovingEnemy::handleCollision(Player& player) {
    if (isWeak()) {
        std::cout << "Weak enemy killed by player!" << std::endl;
        setActive(false); // Deactivate the enemy
        GameState::getInstance()->addScore(10); // Use GameState for score tracking
    }
    else {
        std::cout << "Player killed by enemy!" << std::endl;
        player.setDead(); // Kill the player if enemy is strong
    }
}

