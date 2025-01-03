#include "Player.h"
#include "graphics.h"
#include "GameState.h"
#include "MovingEnemy.h"   // Fixed: Use MovingEnemy instead of Enemy
#include <iostream>  // For debug output

const float CELL_SIZE = 50.0f; // Match grid cell size

// Constructor
Player::Player(GameState* gs, int startX, int startY, float speed)
    : GameObject(gs, "Player"), gridX(startX), gridY(startY), directionX(0), directionY(-1),
    nextDirectionX(0), nextDirectionY(-1), speed(speed), moving(false), isAlive(true), hitEdge(false) {
    x = startX * CELL_SIZE + CELL_SIZE / 2;
    y = startY * CELL_SIZE + CELL_SIZE / 2;
    targetX = x;
    targetY = y;

    // Player starts with no tail
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

    // Clear tail on reset
    tail.clear();
}

// Add a tail segment
void Player::addTailSegment() {
    int newGridX, newGridY;
    float newX, newY, newTargetX, newTargetY;

    if (tail.empty()) {
        newGridX = gridX - directionX;
        newGridY = gridY - directionY;
    }
    else {
        const auto& lastSegment = tail.back();
        newGridX = lastSegment.gridX - directionX;
        newGridY = lastSegment.gridY - directionY;
    }

    newX = newGridX * CELL_SIZE + CELL_SIZE / 2;
    newY = newGridY * CELL_SIZE + CELL_SIZE / 2;
    newTargetX = newX;
    newTargetY = newY;

    tail.push_back({ newGridX, newGridY, newX, newY, newTargetX, newTargetY });
    std::cout << "Tail segment added. Total segments: " << tail.size() << std::endl;
}

void Player::shedTail() {
    if (tail.empty()) return; // No tail to shed

    std::cout << "Shedding tail: " << tail.size() << " segments." << std::endl;

    for (const auto& segment : tail) {
        // FIXED: Spawn MovingEnemy instead of abstract Enemy
        MovingEnemy* enemy = new MovingEnemy(GameState::getInstance(), segment.gridX, segment.gridY);

        // Null check before adding to gameObjects
        if (!enemy) {
            std::cerr << "Failed to create enemy!" << std::endl;
            continue; // Skip this segment if creation failed
        }

        GameState::getInstance()->addObject(enemy); // Add enemy to game objects
        std::cout << "Enemy spawned at: (" << segment.gridX << ", " << segment.gridY << ")" << std::endl;
    }

    // Clear tail after shedding
    tail.clear();
}


// Update movement and collisions
void Player::update(float dt) {
    if (!isAlive) {
        return; // Stop updating if player is dead
    }

    handleInput(); // Handle input for movement

    // Detect space key to shed tail
    if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
        shedTail(); // Trigger shedding mechanic
    }

    // Move smoothly toward the target
    moveToTarget(dt);

    // Smooth tail movement
    updateTail(dt); // Update tail with smooth movement

    // Check for self-collision with tail
    checkTailCollision();

    // If reached the target, check for the next move
    if (!moving) {
        if (!tail.empty()) {
            for (int i = tail.size() - 1; i > 0; --i) {
                tail[i].gridX = tail[i - 1].gridX;
                tail[i].gridY = tail[i - 1].gridY;
                tail[i].targetX = tail[i - 1].targetX;
                tail[i].targetY = tail[i - 1].targetY;
            }
            tail[0].gridX = gridX;
            tail[0].gridY = gridY;
            tail[0].targetX = targetX;
            tail[0].targetY = targetY;
        }

        directionX = nextDirectionX;
        directionY = nextDirectionY;

        gridX += directionX;
        gridY += directionY;

        checkCollision();

        if (isAlive) {
            targetX = gridX * CELL_SIZE + CELL_SIZE / 2;
            targetY = gridY * CELL_SIZE + CELL_SIZE / 2;
            moving = true;
        }
    }
}

// Move smoothly toward the target
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

// Check for collision with the tail
void Player::checkTailCollision() {
    for (const auto& segment : tail) {
        if (segment.gridX == gridX && segment.gridY == gridY) {
            setDead(); // Player dies if colliding with its own tail
            break;
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

    brush.fill_color[0] = 1.0f; // Yellow for tail
    brush.fill_color[1] = 1.0f;
    brush.fill_color[2] = 0.0f;

    for (const auto& segment : tail) {
        graphics::drawRect(segment.x, segment.y, CELL_SIZE * 0.8f, CELL_SIZE * 0.8f, brush);
    }
}

void Player::handleInput() {
    if (graphics::getKeyState(graphics::SCANCODE_UP) && directionY == 0) nextDirectionX = 0, nextDirectionY = -1;
    else if (graphics::getKeyState(graphics::SCANCODE_DOWN) && directionY == 0) nextDirectionX = 0, nextDirectionY = 1;
    else if (graphics::getKeyState(graphics::SCANCODE_LEFT) && directionX == 0) nextDirectionX = -1, nextDirectionY = 0;
    else if (graphics::getKeyState(graphics::SCANCODE_RIGHT) && directionX == 0) nextDirectionX = 1, nextDirectionY = 0;
}

void Player::checkCollision() {
    if (gridX < 0 || gridX >= 12 || gridY < 0 || gridY >= 12) setDead();
}

void Player::setDead() {
    isAlive = false;
    hitEdge = true;
    GameState::getInstance()->endGame();
}

bool Player::getIsAlive() const {
    return isAlive;
}
