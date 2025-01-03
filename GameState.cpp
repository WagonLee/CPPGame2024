#include "GameState.h"
#include <iostream>  // Debug output added
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "MovingEnemy.h"
#include "StationaryEnemy.h" // Added for stationary enemies
#include "Collectible.h"
#include "PowerUpBlue.h"

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
        spawnInteractiveObject<MovingEnemy>();
        lastMovingEnemySpawnTime = currentTime;
        movingEnemySpawnInterval = movingEnemySpawnMin + (rand() / (RAND_MAX / (movingEnemySpawnMax - movingEnemySpawnMin)));
    }

    // --- STATIONARY ENEMY SPAWNING ---
    if (difftime(currentTime, lastStationarySpawnTime) >= stationaryEnemySpawnInterval) {
        spawnInteractiveObject<StationaryEnemy>();
        lastStationarySpawnTime = currentTime;
        stationaryEnemySpawnInterval = stationarySpawnMin + (rand() / (RAND_MAX / (stationarySpawnMax - stationarySpawnMin)));
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

    // --- POWERUP SPAWNING ---
    if (difftime(currentTime, lastPowerUpSpawnTime) >= powerUpSpawnInterval) {
        spawnInteractiveObject<PowerUpBlue>();
        lastPowerUpSpawnTime = currentTime;
        powerUpSpawnInterval = powerUpSpawnMin + (rand() / (RAND_MAX / (powerUpSpawnMax - powerUpSpawnMin)));
    }

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

        // Check collisions
        for (auto& obj : gameObjects) {
            InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());

            // Validate pointer and active state
            if (!interactive || !interactive->isActive()) continue;

            // Perform collision check
            if (interactive->checkCollision(*player)) {
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
        std::remove_if(
            gameObjects.begin(),
            gameObjects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return !obj || !obj->isActive();
            }),
        gameObjects.end());

    // --- Debug Logs ---
#ifdef DEBUG
    std::cout << "Active objects after update: " << gameObjects.size() << std::endl;
#endif
}

// Draw all game objects
void GameState::draw() {
    for (auto& obj : gameObjects) {
        if (obj->isActive()) {
            obj->draw();
        }
    }
}

// Initialize game state
void GameState::init() {
    lastMovingEnemySpawnTime = time(nullptr);
    movingEnemySpawnInterval = movingEnemySpawnMin + (rand() / (RAND_MAX / (movingEnemySpawnMax - movingEnemySpawnMin)));

    lastStationarySpawnTime = time(nullptr); // Added for stationary enemies
    stationaryEnemySpawnInterval = stationarySpawnMin + (rand() / (RAND_MAX / (stationarySpawnMax - stationarySpawnMin)));

    lastPowerUpSpawnTime = time(nullptr);
    srand(static_cast<unsigned int>(time(nullptr)));

    powerUpSpawnInterval = powerUpSpawnMin + (rand() / (RAND_MAX / (powerUpSpawnMax - powerUpSpawnMin)));

    // Spawn initial collectibles
    for (int i = 0; i < collectibleCount; i++) {
        spawnInteractiveObject<Collectible>();
    }
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
