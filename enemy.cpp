#include "Enemy.h"
#include "graphics.h"
#include "GameState.h"
#include <cstdlib>   // For rand()
#include <iostream>  // For debug logs

const float CELL_SIZE = 50.0f; // Match grid cell size

// Constructor
Enemy::Enemy(GameState* state, int x, int y)
    : InteractiveObject(state, x, y, "Enemy"), isWeak(false), directionX(0), directionY(0), moveInterval(1500 + rand() % 2500) {
    lastMoveTime = std::chrono::high_resolution_clock::now(); // Initialize movement timer

    // Smooth movement variables
    xPos = x * CELL_SIZE + CELL_SIZE / 2;
    yPos = y * CELL_SIZE + CELL_SIZE / 2;
    targetX = xPos;
    targetY = yPos;
    moving = false;
    speed = 0.002f; // Match the player's speed

    randomizeDirection(); // Set initial random direction
}

// Draw the enemy
void Enemy::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    if (isWeak) {
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
void Enemy::update(float dt) {
    // Smooth movement
    moveToTarget(dt);

    // Handle movement timing
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime).count();

    if (!moving && elapsed >= moveInterval) { // Random move interval
        lastMoveTime = now; // Reset timer
        moveInterval = 1500 + rand() % 2500; // Randomize next move interval (1.5s - 4s)

        // Pick a new random direction
        randomizeDirection();

        // Calculate new grid position
        int newGridX = gridX + directionX;
        int newGridY = gridY + directionY;

        // Check if the new position is valid
        if (canMoveTo(newGridX, newGridY)) { // NEW CHECK: Validate move
            gridX = newGridX;
            gridY = newGridY;

            // Set target position for smooth movement
            targetX = gridX * CELL_SIZE + CELL_SIZE / 2;
            targetY = gridY * CELL_SIZE + CELL_SIZE / 2;
            moving = true;

            std::cout << "Enemy moved to (" << gridX << ", " << gridY << ") with interval "
                << moveInterval << " ms in direction (" << directionX << ", " << directionY << ")" << std::endl;
        }
        else {
            // Retry direction if move is invalid
            randomizeDirection();
        }
    }
}

// Move smoothly toward the target
void Enemy::moveToTarget(float dt) {
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
            moving = false; // Reached target
        }
    }
}

// Initialize behavior
void Enemy::init() {
    randomizeDirection(); // Initialize direction
}

// Handle collision
void Enemy::handleCollision(Player& player) {
    if (isWeak) {
        setActive(false); // Enemy disappears if weak
    }
    else {
        player.setDead(); // Player dies if enemy is strong
    }
}

// Randomize direction
void Enemy::randomizeDirection() {
    int dir = rand() % 4; // 0 = up, 1 = down, 2 = left, 3 = right
    switch (dir) {
    case 0: directionX = 0; directionY = -1; break; // Up
    case 1: directionX = 0; directionY = 1; break;  // Down
    case 2: directionX = -1; directionY = 0; break; // Left
    case 3: directionX = 1; directionY = 0; break;  // Right
    }
    std::cout << "Enemy changed direction: (" << directionX << ", " << directionY << ")" << std::endl;
}

// Check if the move is valid (NEW FUNCTION)
bool Enemy::canMoveTo(int x, int y) const {
    // Ensure position is within bounds
    if (x < 0 || x >= 12 || y < 0 || y >= 12) {
        return false;
    }

    // Check for collisions with other interactive objects
    for (const auto& obj : GameState::getInstance()->getGameObjects()) {
        InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
        if (interactive && interactive->isActive() &&
            interactive->getGridX() == x && interactive->getGridY() == y) {
            return false; // Blocked by another object
        }
    }

    return true; // Position is valid
}
