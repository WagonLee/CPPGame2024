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
#include "PowerUpBase.h"
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

    // Player death state
    bool isGameOver = false;

    GameState();

    // Private methods
    void spawnDepositZone();  // Handles spawning deposit zones

    int score = 0;         // Total score
    int tally = 0;         // Tracks consecutive deposits

    // Add a pointer for the active power-up
    std::unique_ptr<PowerUpBase> activePowerUp = nullptr;

    // Power-up spawning thresholds
    const int tallyLevel1 = 6;
    const int tallyLevel2 = 8;
    const int tallyLevel3 = 10;

    // Power-up spawn handler
    void spawnPowerUp(int level);

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

    const std::unique_ptr<DepositZone>& getDepositZone() const { return depositZone; }

    // Score management
    void addScore(int points); // Adds points to score (general purpose)
    int getScore() const { return score; } // Getter for score

    // Tally management
    void incrementTally(int count); // Increments tally (only deposits)
    int getTally() const { return tally; } // Getter for tally
    void resetTally(); // Resets tally for power-ups

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
    else if constexpr (std::is_same<T, PowerUpLevel1>::value) { // Level 1 Power-Up
        addObject(new PowerUpLevel1(this, gridX, gridY));
    }
    else if constexpr (std::is_same<T, PowerUpLevel2>::value) { // Level 2 Power-Up
        addObject(new PowerUpLevel2(this, gridX, gridY));
    }
    else if constexpr (std::is_same<T, PowerUpLevel3>::value) { // Level 3 Power-Up
        addObject(new PowerUpLevel3(this, gridX, gridY));
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
