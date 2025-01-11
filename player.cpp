#include "Player.h"
#include "graphics.h"
#include "GameState.h"
#include "MovingEnemy.h"
#include <iostream>
#include "config.h"

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
    directionY = -1;
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

    if (tail.empty()) {
        newGridX = gridX - directionX;
        newGridY = gridY - directionY;
    }
    else {
        const auto& lastSegment = tail.back();
        newGridX = lastSegment.gridX - directionX;
        newGridY = lastSegment.gridY - directionY;
    }

    float newX = newGridX * CELL_SIZE + CELL_SIZE / 2;
    float newY = newGridY * CELL_SIZE + CELL_SIZE / 2;

    tail.push_back({ newGridX, newGridY, newX, newY, newX, newY });
    std::cout << "Tail segment added. Total segments: " << tail.size() << std::endl;
}

void Player::shedTail() {
    if (tail.empty()) return; // No tail to shed

    auto* depositZone = GameState::getInstance()->getDepositZone().get();
    std::cout << "Processing tail: " << tail.size() << " segments." << std::endl;

    std::vector<TailSegment> tempTail = tail;
    tail.clear(); // Clear tail immediately to avoid reprocessing
    int depositedCount = 0;

    // Check if any power-up is active
    bool powerUpActive = GameState::getInstance()->isAnyPowerUpActive();

    for (const auto& segment : tempTail) {
        // Check if segment is inside the zone
        if (depositZone->isTileInZone(segment.gridX, segment.gridY)) {
            std::cout << "Segment at (" << segment.gridX << ", " << segment.gridY
                << ") deposited." << std::endl;
            depositedCount++;
        }
        else {
            // Spawn enemy for non-deposited segment
            std::cout << "Segment at (" << segment.gridX << ", " << segment.gridY
                << ") -> enemy" << std::endl;

            // Spawn MovingEnemy
            MovingEnemy* enemy = new MovingEnemy(GameState::getInstance(), segment.gridX, segment.gridY);
            GameState::getInstance()->addObject(enemy);

            // Set enemy to weak state if power-up is active
            if (powerUpActive) {
                enemy->setWeak(true);  // Enemy starts in a weak state
                enemy->stopMovement(); // Optional: Stop movement for weak enemies
                std::cout << "Enemy spawned in a WEAK state at (" << segment.gridX
                    << ", " << segment.gridY << ")" << std::endl;
            }
        }
    }

    // Update tally and score AFTER processing all segments
    if (depositedCount > 0) {
        GameState::getInstance()->incrementTally(depositedCount);
        GameState::getInstance()->addScore(depositedCount);

        // Force respawn AFTER processing all deposits
        std::cout << "Forcing deposit zone respawn immediately!" << std::endl;
        GameState::getInstance()->replaceDepositZone();
    }

    std::cout << "Tail processing complete. Deposited: " << depositedCount << std::endl;
}

// Update movement and collisions
void Player::update(float dt) {
    if (!isAlive) return;

    handleInput();

    if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
        shedTail();
    }

    // Move smoothly toward the target
    moveToTarget(dt);
    updateTail(dt);
    // Smooth tail movement
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
    for (auto& segment : tail) {
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
            setDead();
            break;
        }
    }
}

// Draw the player and its tail
void Player::draw() {
    graphics::Brush brush;

    // Choose texture based on player state
    if (hitEdge || !isAlive) {
        brush.texture = ASSET_PATH + "CHAR-C.png"; // Texture for death
    }
    else {
        brush.texture = ASSET_PATH + "CHAR-D.png"; // Texture for alive state
    }

    brush.outline_opacity = 0.0f; // Remove outlines
    brush.fill_opacity = 1.0f;

    // Draw the player with the selected texture
    graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, brush);

    // Tail rendering (still using color)
    brush.texture = ""; // No texture for tail, fallback to color
    brush.fill_color[0] = 1.0f; // Yellow tail
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
    // Adjust boundaries to account for the shifted playable area
    if (gridX < 1 || gridX >= 1 + PLAYABLE_COLUMNS || gridY < UI_ROWS_ABOVE || gridY >= UI_ROWS_ABOVE + PLAYABLE_ROWS) {
        setDead();
    }
}

void Player::setDead() {
    isAlive = false;
    hitEdge = true;
    GameState::getInstance()->endGame();
}

bool Player::getIsAlive() const {
    return isAlive;
}