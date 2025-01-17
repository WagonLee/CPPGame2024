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

    std::vector<TailSegment> tempTail = tail;
    tail.clear(); // Clear tail immediately to avoid reprocessing
    int depositedCount = 0;

    // Check if any power-up is active
    bool powerUpActive = GameState::getInstance()->isAnyPowerUpActive();

    float lingerBaseTime = 0.1f; // Base linger duration per collectible
    float currentLingerTime = 0.0f; // Incremental linger time for each collectible

    for (const auto& segment : tempTail) {
        // Check if segment is inside the zone
        if (depositZone->isTileInZone(segment.gridX, segment.gridY)) {
            depositedCount++;

            // Find collectible at the deposited location
            for (const auto& obj : GameState::getInstance()->getGameObjects()) {
                Collectible* collectible = dynamic_cast<Collectible*>(obj.get());
                if (collectible && collectible->getGridX() == segment.gridX &&
                    collectible->getGridY() == segment.gridY) {
                    currentLingerTime += lingerBaseTime; // Increment linger time
                    collectible->startLingering(currentLingerTime * 1000.0f); // Convert to milliseconds
                }
            }
        }
        else {
            // Spawn enemy for non-deposited segment
            MovingEnemy* enemy = new MovingEnemy(GameState::getInstance(), segment.gridX, segment.gridY);
            GameState::getInstance()->addObject(enemy);

            // Set enemy to weak state if power-up is active
            if (powerUpActive) {
                enemy->setWeak(true);  // Enemy starts in a weak state
                enemy->stopMovement(); // Optional: Stop movement for weak enemies
            }
        }
    }

    // Update tally and score AFTER processing all segments
    if (depositedCount > 0) {
        GameState* gameState = GameState::getInstance();

        // Revamped scoring logic
        int depositScore = 0;
        for (int i = 1; i <= depositedCount; ++i) {
            depositScore += i * 10; // Add score based on the deposit pattern
        }

        gameState->incrementTally(depositedCount); // Increment tally
        gameState->addScore(depositScore);         // Update game score

        // Force respawn AFTER processing all deposits
        gameState->replaceDepositZone();

        std::cout << "Deposited " << depositedCount << " segments, awarded " << depositScore
            << " points." << std::endl;
    }
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

void Player::draw() {
    graphics::Brush br;

    // Tail rendering with texture 
    br.texture = ASSET_PATH + "objects/COLLECTIBLE.png"; // Tail texture
    br.fill_opacity = 1.0f; 
    br.outline_opacity = 0.0f;

    for (const auto& segment : tail) {
        graphics::drawRect(segment.x, segment.y, CELL_SIZE, CELL_SIZE, br);
    }

    // Determine texture based on movement direction and player state
    if (hitEdge || !isAlive) {
        // Death textures based on last direction
        if (directionX > 0) br.texture = ASSET_PATH + "objects/PLAYER-R-PWND.png";
        else if (directionX < 0) br.texture = ASSET_PATH + "objects/PLAYER-L-PWND.png";
        else if (directionY > 0) br.texture = ASSET_PATH + "objects/PLAYER-D-PWND.png";
        else if (directionY < 0) br.texture = ASSET_PATH + "objects/PLAYER-U-PWND.png";
    }
    else {
        // Alive textures based on current direction
        if (directionX > 0) br.texture = ASSET_PATH + "objects/PLAYER-R.png";
        else if (directionX < 0) br.texture = ASSET_PATH + "objects/PLAYER-L.png";
        else if (directionY > 0) br.texture = ASSET_PATH + "objects/PLAYER-D.png";
        else if (directionY < 0) br.texture = ASSET_PATH + "objects/PLAYER-U.png";
    }

    br.outline_opacity = 0.0f;
    br.fill_opacity = 1.0f;

    // Draw the player with the selected texture (draw this last to appear on top of the tail)
    graphics::drawRect(x, y - CELL_SIZE / 2, CELL_SIZE, CELL_SIZE * 2.0f, br);
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