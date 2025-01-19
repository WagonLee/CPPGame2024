#include "player.h"
#include "graphics.h"
#include "gamestate.h"
#include "movingenemy.h"
#include <iostream>
#include "config.h"

Player::Player(GameState* gs, int startX, int startY, float speed)
    : GameObject(gs, "Player"), gridX(startX), gridY(startY), directionX(0), directionY(-1),
    nextDirectionX(0), nextDirectionY(-1), speed(speed), moving(false), isAlive(true), hitEdge(false) {
    x = startX * CELL_SIZE + CELL_SIZE / 2;
    y = startY * CELL_SIZE + CELL_SIZE / 2;
    targetX = x;
    targetY = y;
}

void Player::init() {
    directionX = 0;
    directionY = -1;
    nextDirectionX = 0;
    nextDirectionY = -1;
    moving = false;
    isAlive = true;
    hitEdge = false;

    gridX = 6;
    gridY = 6;
    x = gridX * CELL_SIZE + CELL_SIZE / 2;
    y = gridY * CELL_SIZE + CELL_SIZE / 2;
    targetX = x;
    targetY = y;

    tail.clear();
}

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
    if (tail.empty()) return; 

    auto* depositZone = GameState::getInstance()->getDepositZone().get();
    std::cout << "Processing tail: " << tail.size() << " segments." << std::endl;

    std::vector<TailSegment> tempTail = tail;
    tail.clear(); 
    int depositedCount = 0;

    bool powerUpActive = GameState::getInstance()->isAnyPowerUpActive();

    for (const auto& segment : tempTail) {
        if (depositZone->isTileInZone(segment.gridX, segment.gridY)) {
            std::cout << "Segment at (" << segment.gridX << ", " << segment.gridY
                << ") deposited." << std::endl;
            depositedCount++;
        }
        else {
            std::cout << "Segment at (" << segment.gridX << ", " << segment.gridY
                << ") -> enemy" << std::endl;

            // Spawn MovingEnemy
            MovingEnemy* enemy = new MovingEnemy(GameState::getInstance(), segment.gridX, segment.gridY);
            GameState::getInstance()->addObject(enemy);

            // Set enemy to weak state if power-up is active
            if (powerUpActive) {
                enemy->setWeak(true);  
                enemy->stopMovement(); 
                std::cout << "Enemy spawned in a WEAK state at (" << segment.gridX
                    << ", " << segment.gridY << ")" << std::endl;
            }
        }
    }

    // Update tally and score AFTER processing all segments
    if (depositedCount > 0) {
        GameState* gameState = GameState::getInstance();

        graphics::playSound(ASSET_PATH + "sounds/depo.wav", 1.0f, false);

        int depositScore = 0;
        for (int i = 1; i <= depositedCount; ++i) {
            depositScore += i * 10; // Add score based on the deposit combo
        }

        gameState->incrementTally(depositedCount); 
        gameState->addScore(depositScore);         

        std::cout << "Forcing deposit zone respawn immediately!" << std::endl;
        gameState->replaceDepositZone();

        std::cout << "Deposited " << depositedCount << " segments, awarded " << depositScore
            << " points (Pattern scoring applied)." << std::endl;
    }
    else {
        graphics::playSound(ASSET_PATH + "sounds/faildepo.wav", 1.0f, false);
        std::cout << "Failed to deposit any segments." << std::endl;
    }

    std::cout << "Tail processing complete. Deposited: " << depositedCount << std::endl;
}

void Player::update(float dt) {
    if (!isAlive) return;

    handleInput();

    if (graphics::getKeyState(graphics::SCANCODE_SPACE) || graphics::getKeyState(graphics::SCANCODE_X)) {
        shedTail();
    }

    // Move smoothly
    moveToTarget(dt);
    updateTail(dt);
    checkTailCollision();

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

    graphics::drawRect(x, y - CELL_SIZE / 2, CELL_SIZE, CELL_SIZE * 2.0f, br);
}

void Player::handleInput() {
    if (graphics::getKeyState(graphics::SCANCODE_UP) && directionY == 0) nextDirectionX = 0, nextDirectionY = -1;
    else if (graphics::getKeyState(graphics::SCANCODE_DOWN) && directionY == 0) nextDirectionX = 0, nextDirectionY = 1;
    else if (graphics::getKeyState(graphics::SCANCODE_LEFT) && directionX == 0) nextDirectionX = -1, nextDirectionY = 0;
    else if (graphics::getKeyState(graphics::SCANCODE_RIGHT) && directionX == 0) nextDirectionX = 1, nextDirectionY = 0;
}

void Player::checkCollision() {
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