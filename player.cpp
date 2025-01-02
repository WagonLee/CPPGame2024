#include "Player.h"
#include "graphics.h"
#include "GameState.h"

const float CELL_SIZE = 50.0f; // Match grid cell size

// Constructor
Player::Player(GameState* gs, int startX, int startY, float speed)
    : GameObject(gs, "Player"), gridX(startX), gridY(startY), directionX(0), directionY(-1),
    nextDirectionX(0), nextDirectionY(-1), speed(speed), moving(false), isAlive(true), hitEdge(false) {
    x = startX * CELL_SIZE + CELL_SIZE / 2; // Exact pixel position
    y = startY * CELL_SIZE + CELL_SIZE / 2;
    targetX = x;
    targetY = y;
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
}

// Update movement and collisions
void Player::update(float dt) {
    if (!isAlive) {
        return; // Stop updating if player is dead
    }

    handleInput();

    // Move smoothly toward the target
    moveToTarget(dt);

    // If reached the target, check for the next move
    if (!moving) {
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
        // Move toward the target position smoothly
        float dx = targetX - x;
        float dy = targetY - y;
        float dist = sqrt(dx * dx + dy * dy);
        float step = speed * CELL_SIZE * dt;

        if (dist > step) {
            // Move by step size
            x += step * dx / dist;
            y += step * dy / dist;
        }
        else {
            // Snap to target if close enough
            x = targetX;
            y = targetY;
            moving = false; // Stop moving
        }
    }
}

// Draw the player
void Player::draw() {
    graphics::Brush brush;

    if (hitEdge || !isAlive) {
        // Turn pink if the player hit the edge or is dead
        brush.fill_color[0] = 1.0f; // Pink (R)
        brush.fill_color[1] = 0.0f; // Pink (G)
        brush.fill_color[2] = 1.0f; // Pink (B)
    }
    else {
        // Default color (green)
        brush.fill_color[0] = 0.0f;
        brush.fill_color[1] = 1.0f;
        brush.fill_color[2] = 0.0f;
    }

    brush.fill_opacity = 1.0f;
    graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, brush);
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
    // Check for hitting the edge of the grid
    if (gridX < 0 || gridX >= 12 || gridY < 0 || gridY >= 12) {
        setDead(); // Mark player as dead
    }
}

// Set player as dead and stop activity
void Player::setDead() {
    isAlive = false;
    hitEdge = true;

    // Notify GameState to stop updates
    GameState::getInstance()->endGame();
}

// Return whether the player is alive
bool Player::getIsAlive() const {
    return isAlive;
}
