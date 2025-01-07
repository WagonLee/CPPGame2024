#include "GameState.h"
#include "DepositZone.h"
#include <iostream>  // Debug output added
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "MovingEnemy.h"
#include "StationaryEnemy.h"
#include "Collectible.h"
#include "Config.h"
#include "PowerUpLevel1.h" // Include Level 1 Power-Up
#include "PowerUpLevel2.h" // Include Level 2 Power-Up
#include "PowerUpLevel3.h" // Include Level 3 Power-Up

GameState* GameState::instance = nullptr;

// Constructor
GameState::GameState() {}

// Singleton instance
GameState* GameState::getInstance() {
    if (!instance) {
        instance = new GameState();
    }
    return instance;
}

// Add object to game
void GameState::addObject(GameObject* obj) {
    if (!obj) { // Null check to prevent crashes
        std::cerr << "Error: Attempted to add null object to gameObjects!" << std::endl;
        return;
    }

    if (!obj->isActive()) { // Validate active state before adding
        std::cerr << "Error: Attempted to add inactive object to gameObjects!" << std::endl;
        return;
    }

    gameObjects.emplace_back(obj); // Add valid object
}

// --- Deposit Zone Handling ---

// Spawn deposit zone
void GameState::spawnDepositZone() {
    int gridX, gridY;
    DepositZone::Shape shape;

    // Random position for deposit zone
    gridX = rand() % 9; // Ensures valid positions for larger shapes
    gridY = rand() % 9;

    // Random shape
    int shapeIndex = rand() % 3;
    switch (shapeIndex) {
    case 0: shape = DepositZone::Shape::STRAIGHT_LINE; break;
    case 1: shape = DepositZone::Shape::DONUT; break;
    case 2: shape = DepositZone::Shape::CIRCLE; break;
    }

    bool horizontal = rand() % 2 == 0; // Horizontal or vertical line

    // Create new deposit zone
    depositZone = std::make_unique<DepositZone>(this, gridX, gridY, shape, horizontal);
}

void GameState::spawnPowerUp(int level) {
    std::cout << "spawnPowerUp() called for Level " << level << std::endl; // Debug log
    int gridX, gridY;
    bool positionValid = false;

    // Attempt to find a valid position
    for (int attempts = 0; attempts < 100; ++attempts) {
        gridX = rand() % 12; // Random X position
        gridY = rand() % 12; // Random Y position

        std::cout << "Attempt " << attempts + 1 << ": Checking (" << gridX << ", " << gridY << ")" << std::endl;

        positionValid = true;
        for (const auto& obj : gameObjects) {
            InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
            if (interactive && interactive->isActive() &&
                interactive->getGridX() == gridX && interactive->getGridY() == gridY) {
                positionValid = false;
                std::cout << "Position (" << gridX << ", " << gridY << ") is occupied." << std::endl;
                break;
            }
        }

        if (positionValid) {
            std::cout << "Valid position found: (" << gridX << ", " << gridY << ")" << std::endl;
            break; // Exit loop on valid position
        }
    }

    // If no valid position, force placement at (5, 5)
    if (!positionValid) {
        gridX = 5;
        gridY = 5;
        std::cerr << "WARNING: Forced spawn at (5, 5) for Power-Up Level " << level << std::endl;
    }

    // Attempt to create the power-up
    std::unique_ptr<PowerUpBase> powerUp = nullptr;
    switch (level) {
    case 1:
        powerUp = std::make_unique<PowerUpLevel1>(this, gridX, gridY);
        break;
    case 2:
        powerUp = std::make_unique<PowerUpLevel2>(this, gridX, gridY);
        break;
    case 3:
        powerUp = std::make_unique<PowerUpLevel3>(this, gridX, gridY);
        break;
    }

    // Final validation and object addition
    if (powerUp) {
        std::cout << "Power-Up object created successfully." << std::endl;
        powerUp->init(); // Initialize the object
        activePowerUps.push_back(std::move(powerUp));
        addObject(activePowerUps.back().get());
        std::cout << "Spawned Power-Up Level " << level << " at (" << gridX << ", " << gridY << ")" << std::endl;
    }
    else {
        std::cerr << "Failed to create Power-Up Level " << level << std::endl;
    }
}

void GameState::updatePowerUpTimers(float dt) {
    for (auto& timer : upgradeTimers) {
        timer.second += dt; // Increment timer

        PowerUpBase* powerUp = timer.first->get();

        // Upgrade when time exceeds threshold
        if (timer.second >= upgradeTime && powerUp->isActive()) {
            int newLevel = powerUp->getLevel() + 1;

            if (newLevel <= 3) { // Upgrade only if level < 3
                std::cout << "Upgrading Power-Up to Level " << newLevel << std::endl;

                // Replace with new upgraded power-up
                spawnPowerUp(newLevel);
                powerUp->setActive(false); // Deactivate old power-up
            }
            timer.second = 0.0f; // Reset timer for upgraded power-up
        }
    }

    // Remove timers for inactive power-ups
    upgradeTimers.erase(
        std::remove_if(upgradeTimers.begin(), upgradeTimers.end(),
            [](const std::pair<std::unique_ptr<PowerUpBase>*, float>& t) { return !t.first->get()->isActive(); }),
        upgradeTimers.end());
}

// Update game state
    void GameState::update(float dt) {

    if (isGameOver) return; // Stop updates after player death

    // --- Preallocate memory to avoid resizing ---
    if (gameObjects.capacity() < 100) {
        gameObjects.reserve(100);
    }

    // --- MOVING ENEMY SPAWNING ---
    time_t currentTime = time(nullptr);
    if (difftime(currentTime, lastMovingEnemySpawnTime) >= movingEnemySpawnInterval) {
        //spawnInteractiveObject<MovingEnemy>();
        lastMovingEnemySpawnTime = currentTime;
        movingEnemySpawnInterval = movingEnemySpawnMin + (rand() / (RAND_MAX / (movingEnemySpawnMax - movingEnemySpawnMin)));
    }

    // --- STATIONARY ENEMY SPAWNING ---
    if (difftime(currentTime, lastStationarySpawnTime) >= stationaryEnemySpawnInterval) {
        //spawnInteractiveObject<StationaryEnemy>();
        lastStationarySpawnTime = currentTime;
        stationaryEnemySpawnInterval = stationarySpawnMin + (rand() / (RAND_MAX / (stationarySpawnMax - stationarySpawnMin)));
    }

    // --- DEPOSIT ZONE HANDLING ---
    if (!depositZone) {
        spawnDepositZone(); // First-time spawn
    }
    else {
        depositZone->update(dt); // Update zone timer

        // Smooth despawn transition when expired
        if (depositZone->isExpired()) {
            std::cout << "Deposit zone expired! Respawning..." << std::endl;

            // Log expiration and clear tiles for debugging
            for (const auto& tile : depositZone->getTiles()) {
                std::cout << "Tile: (" << tile.first << ", " << tile.second << ")" << std::endl;
            }

            // Reset and respawn
            depositZone->resetTimer(); // Reset timer for new zone
            spawnDepositZone();        // Create a new zone immediately
        }
    }

    // --- COLLECTIBLE RESPAWN MANAGEMENT ---
    for (auto it = collectibleRespawnTimers.begin(); it != collectibleRespawnTimers.end();) {
        if (difftime(currentTime, it->second) >= collectibleRespawnDelay) {
            spawnInteractiveObject<Collectible>();
            it = collectibleRespawnTimers.erase(it);
        }
        else {
            ++it;
        }
    }

    if (tally >= tallyLevel3) {
        std::cout << "Condition met for Level 3 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(3);
    }
    else if (tally >= tallyLevel2) {
        std::cout << "Condition met for Level 2 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(2);
    }
    else if (tally >= tallyLevel1) {
        std::cout << "Condition met for Level 1 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(1);
    }

    // Reset Tally AFTER checking ALL conditions
    if (tally >= tallyLevel1) { // Only reset if a Power-Up was attempted
        std::cout << "Tally reset! Reached " << tally << " deposits." << std::endl;
        resetTally();
    }

    // --- POWER-UP AUTO-UPGRADE TIMERS ---
    updatePowerUpTimers(dt);

    // --- COLLISION DETECTION ---
    Player* player = nullptr;

    // Locate the player
    for (auto& obj : gameObjects) {
        player = dynamic_cast<Player*>(obj.get());
        if (player) break;
    }

    static int previousTailSize = -1;
    if (player) {
        int currentTailSize = player->getTailSize();
        if (currentTailSize != previousTailSize) {
            std::cout << "Player tail size updated: " << currentTailSize << std::endl;
            previousTailSize = currentTailSize;
        }

        if (depositZone) { // Check tail segments in the deposit zone
            for (const auto& segment : player->tail) {
                if (depositZone->isTileInZone(segment.gridX, segment.gridY)) {
                    std::cout << "Tail segment inside deposit zone: (" << segment.gridX << ", " << segment.gridY << ")" << std::endl;
                }
            }
        }

        // Check collisions
        for (auto& obj : gameObjects) {
            InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
            if (interactive && interactive->isActive() && interactive->checkCollision(*player)) {
                interactive->handleCollision(*player);
            }
        }
    }

    // --- UPDATE AND REMOVE OBJECTS ---
    std::vector<std::unique_ptr<GameObject>> newObjects; // Buffer for new objects

    // Update all objects and buffer new ones
    for (auto& obj : gameObjects) {
        if (obj && obj->isActive()) {
            obj->update(dt);

            // Collect new objects if spawned during update
            auto movingEnemy = dynamic_cast<MovingEnemy*>(obj.get());
            if (movingEnemy && movingEnemy->getIsWeak()) {
                auto newEnemy = std::make_unique<MovingEnemy>(this, movingEnemy->getGridX(), movingEnemy->getGridY());
                if (newEnemy) {
                    newObjects.push_back(std::move(newEnemy));
                }
            }
                }
            }

    // Append buffered objects safely
    for (auto& newObj : newObjects) {
        gameObjects.push_back(std::move(newObj));
    }

    // Remove inactive objects safely
    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const std::unique_ptr<GameObject>& obj) { return !obj->isActive(); }),
        gameObjects.end());

    // --- Debug Logs ---
#ifdef DEBUG
    std::cout << "Active objects after update: " << gameObjects.size() << std::endl;
#endif
        }

// Draw all game objects
void GameState::draw() {

    // Draw deposit zone first (part of the board)
    if (depositZone) {
        depositZone->draw();
    }

    // Draw all other active objects
    for (auto& obj : gameObjects) {
        if (obj->isActive()) {
            obj->draw();
        }
    }

    // Locate the player
    Player* player = nullptr;
    for (auto& obj : gameObjects) {
        player = dynamic_cast<Player*>(obj.get());
        if (player) break;
    }

    if (player) {
        // Set up the font and brush for text
        graphics::setFont(std::string("assets/Arial.ttf")); // Use Arial font
        graphics::Brush textBrush;
        textBrush.fill_color[0] = 1.0f; // Red text
        textBrush.fill_color[1] = 0.0f;
        textBrush.fill_color[2] = 0.0f;
        textBrush.outline_opacity = 0.0f;

        // Display tail size
        std::string tailText = "Tail Size: " + std::to_string(player->getTailSize());
        float xPos = (GRID_SIZE * CELL_SIZE) / 2.0f; // Horizontal center
        float yPos = 30;                             // 30 pixels from the top
        graphics::drawText(xPos, yPos, 30, tailText, textBrush); // Size 30

        // Display score below tail size
        std::string scoreText = "Score: " + std::to_string(getScore());
        graphics::drawText(xPos, yPos + 40, 30, scoreText, textBrush); // Offset by 40 pixels

        // Display tally below score
        std::string tallyText = "Tally: " + std::to_string(getTally());
        graphics::drawText(xPos, yPos + 80, 30, tallyText, textBrush); // Offset by 80 pixels
    }

}


// Initialize game state
void GameState::init() {
    lastMovingEnemySpawnTime = time(nullptr);
    movingEnemySpawnInterval = movingEnemySpawnMin + (rand() / (RAND_MAX / (movingEnemySpawnMax - movingEnemySpawnMin)));

    lastStationarySpawnTime = time(nullptr); // Added for stationary enemies
    stationaryEnemySpawnInterval = stationarySpawnMin + (rand() / (RAND_MAX / (stationarySpawnMax - stationarySpawnMin)));

    srand(static_cast<unsigned int>(time(nullptr)));

    spawnDepositZone(); // Start with a deposit zone

    // Spawn initial collectibles
    for (int i = 0; i < collectibleCount; i++) {
        spawnInteractiveObject<Collectible>();
    }
}

void GameState::addScore(int points) {
    score += points; // Increment total score
    std::cout << "Score updated! Current score: " << score << std::endl;
}

void GameState::incrementTally(int count) {
    tally += count; // Increment the tally
    std::cout << "Tally updated! Current tally: " << tally << std::endl;
}


void GameState::resetTally() {
    tally = 0; // Reset tally to zero
    std::cout << "Tally reset to 0." << std::endl;
}

// Reset game state
void GameState::reset() {}

// Handles stopping activity after player death
void GameState::endGame() {
    isGameOver = true;
    std::cout << "Game Over! All activity stopped." << std::endl;
}

// Schedule collectible respawn
void GameState::scheduleCollectibleRespawn() {
    time_t currentTime = time(nullptr);
    collectibleRespawnTimers.emplace_back(currentTime, currentTime + static_cast<time_t>(collectibleRespawnDelay));
}

// Destructor
GameState::~GameState() {
    gameObjects.clear();
}
