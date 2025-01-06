#pragma once

#include <vector>
#include <memory>
#include <ctime>      // For time-based tracking
#include <iostream>   // For debug output
#include <chrono>     // For precise timing
#include "GameObject.h"
#include "InteractiveObject.h"
#include "MovingEnemy.h"
#include "StationaryEnemy.h"
#include "Collectible.h"
#include "PowerUpBlue.h"
#include "Player.h"
#include "DepositZone.h" // Added DepositZone

class GameState {
private:
    static GameState* instance;
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // MovingEnemy spawn timing
    time_t lastMovingEnemySpawnTime;                     // Renamed for MovingEnemy
    double movingEnemySpawnInterval;                     // Renamed for MovingEnemy
    const double movingEnemySpawnMin = 5.0;              // Minimum spawn interval
    const double movingEnemySpawnMax = 10.0;             // Maximum spawn interval

    // StationaryEnemy spawn timing
    time_t lastStationarySpawnTime;                      // Added for StationaryEnemy
    double stationaryEnemySpawnInterval;                 // Added for StationaryEnemy
    const double stationarySpawnMin = 10.0;              // Minimum spawn interval
    const double stationarySpawnMax = 20.0;              // Maximum spawn interval

    // Deposit zone variables
    std::unique_ptr<DepositZone> depositZone; // Single deposit zone
    const double depositZoneDuration = 10.0; // 10 seconds per zone

    // Collectible management
    const int collectibleCount = 2;

    // Collectible respawn
    std::vector<std::pair<time_t, time_t>> collectibleRespawnTimers; // Tracks respawn timers
    const double collectibleRespawnDelay = 0.1; // Delay in seconds before respawn

    // PowerUp spawn timing
    time_t lastPowerUpSpawnTime;
    double powerUpSpawnInterval;
    const double powerUpSpawnMin = 10.0;
    const double powerUpSpawnMax = 20.0;

    // Player death state
    bool isGameOver = false;

    GameState();

    // Private methods
    void spawnDepositZone();  // Handles spawning deposit zones

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

    // Schedule collectible respawn after a delay
    void scheduleCollectibleRespawn();

    // Provide read-only access to game objects
    const std::vector<std::unique_ptr<GameObject>>& getGameObjects() const {
        return gameObjects;
    }

    // Destructor
    ~GameState();
};

// Template implementation must be in the header file
template <typename T>
void GameState::spawnInteractiveObject() {
    if (isGameOver) return;

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
                positionValid = false;
                break;
            }
        }

        // Prevent spawning on player's tail segments
        Player* player = nullptr;
        for (const auto& obj : gameObjects) {
            player = dynamic_cast<Player*>(obj.get());
            if (player) break;
        }

        if (player) {
            for (const auto& segment : player->tail) {
                if (segment.gridX == gridX && segment.gridY == gridY) {
                    positionValid = false;
                    break;
                }
            }
        }

        attempts++;
    }

    if (!positionValid) return;

    // Correct instantiation for object type
    if constexpr (std::is_same<T, MovingEnemy>::value) {
        addObject(new MovingEnemy(this, gridX, gridY));
    }
    else if constexpr (std::is_same<T, StationaryEnemy>::value) { // Added StationaryEnemy
        addObject(new StationaryEnemy(this, gridX, gridY));
    }
    else if constexpr (std::is_same<T, Collectible>::value) {
        addObject(new Collectible(this, gridX, gridY));
    }
    else if constexpr (std::is_same<T, PowerUpBlue>::value) {
        addObject(new PowerUpBlue(this, gridX, gridY));
    }

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
