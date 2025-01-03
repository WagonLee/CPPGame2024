#pragma once

#include <vector>
#include <memory>
#include <ctime>      // For time-based tracking
#include <iostream>   // For debug output
#include <chrono>     // For precise timing
#include "GameObject.h"
#include "InteractiveObject.h"
#include "Enemy.h"
#include "Collectible.h"
#include "PowerUpBlue.h"
#include "Player.h" // Include player for tail checks

class GameState {
private:
    static GameState* instance;
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // Enemy spawn timing
    time_t lastSpawnTime;
    double enemySpawnInterval;
    const double enemySpawnMin = 5.0;
    const double enemySpawnMax = 10.0;

    // Collectible management
    const int collectibleCount = 2;

    // Collectible respawn
    std::vector<std::pair<time_t, time_t>> collectibleRespawnTimers; // Tracks respawn timers
    const double collectibleRespawnDelay = 0.1; // Delay in seconds before respawn

    // PowerUp spawn timing
    time_t lastPowerUpSpawnTime;
    double powerUpSpawnInterval;
    const double powerUpSpawnMin = 1.0;
    const double powerUpSpawnMax = 2.0;

    // Player death state
    bool isGameOver = false;

    GameState();

public:
    // Singleton pattern
    static GameState* getInstance();

    // Game state management
    void addObject(GameObject* obj);
    void update(float dt);
    void draw();
    void init();
    void reset();
    void endGame(); // Handles stopping all activity on player death

    // Template method for spawning InteractiveObjects
    template <typename T>
    void spawnInteractiveObject();

    // NEW: Schedule collectible respawn after a delay
    void scheduleCollectibleRespawn();

    // NEW: Provide read-only access to game objects (Added for Enemy checks)
    const std::vector<std::unique_ptr<GameObject>>& getGameObjects() const {
        return gameObjects;
    }

    // Destructor
    ~GameState();
};

// Template implementation must be in the header file
template <typename T>
void GameState::spawnInteractiveObject() {
    if (isGameOver) return; // Prevent spawning after player death

    int gridX, gridY;
    bool positionValid = false;
    int attempts = 0;

    while (!positionValid && attempts < 100) {
        gridX = rand() % 12;
        gridY = rand() % 12;
        positionValid = true;

        // Check for conflicts with other active objects
        for (const auto& obj : gameObjects) {
            InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
            if (interactive && interactive->isActive() &&
                interactive->getGridX() == gridX && interactive->getGridY() == gridY) {
                std::cout << "Conflict detected at: (" << gridX << ", " << gridY << ")" << std::endl;
                positionValid = false; // Retry if conflict
                break;
            }
        }

        // NEW CHECK: Prevent spawning on player's tail segments
        Player* player = nullptr;
        for (const auto& obj : gameObjects) {
            player = dynamic_cast<Player*>(obj.get());
            if (player) break; // Found player instance
        }

        if (player) {
            const auto& tail = player->getTailSize(); // Access tail segments
            for (const auto& segment : player->tail) { // Iterate through tail
                if (segment.gridX == gridX && segment.gridY == gridY) {
                    std::cout << "Spawn conflict with tail at: (" << gridX << ", " << gridY << ")" << std::endl;
                    positionValid = false;
                    break;
                }
            }
        }

        attempts++;
    }

    // Abort spawn if no valid position is found
    if (!positionValid) {
        std::cerr << "Failed to find valid spawn position!" << std::endl;
        return;
    }

    // Spawn object if position is valid
    T* obj = new T(this, gridX, gridY);
    addObject(obj);

    // Debug log with timestamp
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    struct tm timeInfo;
    localtime_s(&timeInfo, &timeT);

    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%a %b %d %H:%M:%S", &timeInfo);

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) %
        1000;

    std::cout << typeid(T).name() << " spawned at: (" << gridX << ", " << gridY << ") at time: "
        << timeStr << ":" << ms.count() << " " << (1900 + timeInfo.tm_year) << std::endl;
}
