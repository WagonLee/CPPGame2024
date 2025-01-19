#include "movingenemy.h"
#include "graphics.h"
#include "gamestate.h"
#include <cstdlib>   // for rand()
#include <iostream>  // for debug logs

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

    randomizeDirection(); 

    if (weak) {
        setWeak(true); 
    }
}

// Initialize behavior
void MovingEnemy::init() {
    randomizeDirection(); 
}

// Draw the moving enemy
void MovingEnemy::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    if (isInactive) { 
        br.texture = ASSET_PATH + "objects/MOVING-ENEMY-SPAWNING.png"; 
    }
    else if (isWeak()) { 
        br.texture = ASSET_PATH + "objects/MOVING-ENEMY-WEAK.png"; 
    }
    else {
        br.texture = ASSET_PATH + "objects/MOVING-ENEMY.png"; 
    }

    br.fill_opacity = 1.0f; 

    graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
}

void MovingEnemy::update(float dt) {
    if (isInactive) {
        if (graphics::getGlobalTime() >= inactiveEndTime) {
            isInactive = false;
        }
        return;
    }

    if (isWeak()) {
        return;
    }

    moveToTarget(dt);

    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime).count();

    if (!moving && elapsed >= moveInterval) {
        lastMoveTime = now; 
        moveInterval = 1500 + rand() % 2500; // Randomize move interval (1.5–2.5s)

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

// Move smoothly
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

void MovingEnemy::randomizeDirection() {
    int dir = rand() % 4;
    switch (dir) {
    case 0: directionX = 0; directionY = -1; break; 
    case 1: directionX = 0; directionY = 1; break; 
    case 2: directionX = -1; directionY = 0; break; 
    case 3: directionX = 1; directionY = 0; break; 
    }
}

void MovingEnemy::setWeak(bool weak) {
    isWeakState = weak;
    if (weak) {
        stopMovement(); 
    }
    else {
        startMovement(); 
    }
}

void MovingEnemy::stopMovement() {
    stopped = true;  
    moving = false;  
}

void MovingEnemy::startMovement() {
    stopped = false; 
}

void MovingEnemy::handleCollision(Player& player) {
    if (isInactive) {
        return;
    }

    if (isWeak()) {
        graphics::playSound(ASSET_PATH + "sounds/kill.wav", 0.7f, false);
        std::cout << "Weak enemy killed by player!" << std::endl;
        setActive(false); 

        auto* gameState = GameState::getInstance();

        gameState->addToKillChain(); 
        gameState->enemyKilled = true;
        gameState->enemyKillEndTime = graphics::getGlobalTime() + 1500.0f;
    }
    else {
        std::cout << "Player killed by enemy!" << std::endl;
        player.setDead();
    }
}
