#include "Player.h"
#include "graphics.h"
#include "GameState.h"
#include <iostream> // For debug output

const float CELL_SIZE = 50.0f; // Match grid cell size

// Constructor
Player::Player(GameState* gs, int startX, int startY, float speed)
    : GameObject(gs, "Player"), gridX(startX), gridY(startY), directionX(0), directionY(-1),
    nextDirectionX(0), nextDirectionY(-1), speed(speed), moving(false), isAlive(true), hitEdge(false) {
    x = startX * CELL_SIZE + CELL_SIZE / 2;
    y = startY * CELL_SIZE + CELL_SIZE / 2;
    targetX = x;
    targetY = y;

    // Initialize placeholder tail size of 2 segments
    for (int i = 0; i < 2; ++i) {
        tail.push_back({ startX, startY, x, y, x, y }); // Added smooth tracking variables
    }
}

// Initialize/reset player
void Player::init() {
    directionX = 0;
    directionY = -1; // Start moving up
    nextDirectionX = 0;
    nextDirectionY = -1;
    moving = false;
    isAlive = true;
    hitEdge = false;

    // Reset position
    gridX = 6;
    gridY = 6;
    x = gridX * CELL_SIZE + CELL_SIZE / 2;
    y = gridY * CELL_SIZE + CELL_SIZE / 2;
    targetX = x;
    targetY = y;

    // Clear tail and reinitialize size to 2
    tail.clear();
    for (int i = 0; i < 2; ++i) {
        tail.push_back({ gridX, gridY, x, y, x, y }); // Smooth movement variables added
    }
}

// Update movement and collisions
void Player::update(float dt) {
    if (!isAlive) {
        return; // Stop updating if player is dead
    }

    handleInput(); // Handle input for movement

    // Move smoothly toward the target
    moveToTarget(dt);

    // Smooth tail movement
    updateTail(dt); // Added smooth tail update

    // If reached the target, check for the next move
    if (!moving) {
        // Update tail positions to smoothly follow the player
        if (!tail.empty()) {
            for (int i = tail.size() - 1; i > 0; --i) {
                tail[i].gridX = tail[i - 1].gridX;
                tail[i].gridY = tail[i - 1].gridY;
                tail[i].targetX = tail[i - 1].targetX;
                tail[i].targetY = tail[i - 1].targetY;
            }
            tail[0].gridX = gridX; // First segment follows player's previous position
            tail[0].gridY = gridY;
            tail[0].targetX = targetX;
            tail[0].targetY = targetY;
        }

        // Apply buffered direction change at the move step
        directionX = nextDirectionX;
        directionY = nextDirectionY;

        // Calculate new grid position
        gridX += directionX;
        gridY += directionY;

        // Check for collision with edges
        checkCollision();

        // If still alive, set target position for smooth movement
        if (isAlive) {
            targetX = gridX * CELL_SIZE + CELL_SIZE / 2;
            targetY = gridY * CELL_SIZE + CELL_SIZE / 2;
            moving = true;
        }
    }
}

// Smooth movement to target position
void Player::moveToTarget(float dt) {
    if (moving) {
        float dx = targetX - x;
        float dy = targetY - y;
        float dist = sqrt(dx * dx + dy * dy);
        float step = speed * CELL_SIZE * dt;

        if (dist > step) {
            x += step * dx / dist;
            y += step * dy / dist;
        }
        else {
            x = targetX;
            y = targetY;
            moving = false;
        }
    }
}

// Smooth tail movement
void Player::updateTail(float dt) {
    for (size_t i = 0; i < tail.size(); ++i) {
        TailSegment& segment = tail[i];

        float dx = segment.targetX - segment.x;
        float dy = segment.targetY - segment.y;
        float dist = sqrt(dx * dx + dy * dy);
        float step = speed * CELL_SIZE * dt;

        if (dist > step) {
            segment.x += step * dx / dist;
            segment.y += step * dy / dist;
        }
        else {
            segment.x = segment.targetX;
            segment.y = segment.targetY;
        }
    }
}

// Draw the player and its tail
void Player::draw() {
    graphics::Brush brush;

    if (hitEdge || !isAlive) {
        brush.fill_color[0] = 1.0f; // Pink for death
        brush.fill_color[1] = 0.0f;
        brush.fill_color[2] = 1.0f;
    }
    else {
        brush.fill_color[0] = 0.0f; // Green for player
        brush.fill_color[1] = 1.0f;
        brush.fill_color[2] = 0.0f;
    }

    brush.fill_opacity = 1.0f;
    graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, brush);

    // Draw the tail
    brush.fill_color[0] = 1.0f; // Yellow for tail
    brush.fill_color[1] = 1.0f;
    brush.fill_color[2] = 0.0f;

    for (const auto& segment : tail) {
        graphics::drawRect(segment.x, segment.y, CELL_SIZE * 0.8f, CELL_SIZE * 0.8f, brush); // Smaller tail squares
    }
}

// Handle keyboard input
void Player::handleInput() {
    if (graphics::getKeyState(graphics::SCANCODE_UP) && directionY == 0) {
        nextDirectionX = 0;
        nextDirectionY = -1;
    }
    if (graphics::getKeyState(graphics::SCANCODE_DOWN) && directionY == 0) {
        nextDirectionX = 0;
        nextDirectionY = 1;
    }
    if (graphics::getKeyState(graphics::SCANCODE_LEFT) && directionX == 0) {
        nextDirectionX = -1;
        nextDirectionY = 0;
    }
    if (graphics::getKeyState(graphics::SCANCODE_RIGHT) && directionX == 0) {
        nextDirectionX = 1;
        nextDirectionY = 0;
    }
}

// Collision check with grid edges
void Player::checkCollision() {
    if (gridX < 0 || gridX >= 12 || gridY < 0 || gridY >= 12) {
        setDead();
    }
}

// Death handling
void Player::setDead() {
    isAlive = false;
    hitEdge = true;
    GameState::getInstance()->endGame();
}

bool Player::getIsAlive() const {
    return isAlive;
}
