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
#include "PowerUpLevel1.h" // Include Level 1 Power-Up
#include "PowerUpLevel2.h" // Include Level 2 Power-Up
#include "PowerUpLevel3.h" // Include Level 3 Power-Up
#include "PowerUpLevel4.h" // Include Level 3 Power-Up
#include "Player.h"
#include "DepositZone.h" // Added DepositZone
#include "config.h"

class PowerUpBase;      // Forward declare PowerUpBase
class PowerUpLevel1;    // Forward declare Level 1
class PowerUpLevel2;    // Forward declare Level 2
class PowerUpLevel3;    // Forward declare Level 3
class PowerUpLevel4;    // Forward declare Level 3

class GameState {
private:

    GameState();

    static GameState* instance;
    std::vector<std::unique_ptr<GameObject>> gameObjects;

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

    std::vector<PowerUpBase*> powerUpsToRemove; // Track power-ups marked for removal
    bool isPowerUpRemovalPending = false;      // Flag for pending removal
    bool isProcessingUpdates = false;          // Track if in update loop

    // Private methods
    void spawnDepositZone();  // Handles spawning deposit zones

    int score = 0;         // Total score
    int tally = 0;         // Tracks consecutive deposits

    int killChain = 0;         // Tracks the number of kills in the current chain
    int killChainScore = 0;    // Accumulated score for the chain

    int scoreMulti = 1;

    // Power-Up Management
    std::vector<std::unique_ptr<PowerUpBase>> activePowerUps; // Unlimited power-ups
    std::vector<std::pair<size_t, float>> upgradeTimers;      // Use indices instead of pointers

    void spawnPowerUpAt(int level, int gridX, int gridY);

    const int tallyLevel1 = 6; // Tally thresholds
    const int tallyLevel2 = 7;
    const int tallyLevel3 = 8;
    const int tallyLevel4 = 9;

    const float upgradeTime = 400.0f; // Time for auto-upgrade

    bool preGamePaused = true; // Tracks if the game is in the "READY?" state
    bool paused = false;       // Tracks if the game is paused during gameplay
    int pauseMenuSelection = 0; // Tracks the current option in the pause menu

    bool firstSpawn = true;            // Track the first spawn
    float firstSpawnTime = 0.0f;       // Time for the first spawn 
    bool enemySpawnedInactive = false; // Ensure one spawn at a time

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

    void replaceDepositZone(); // Replaces the deposit zone immediately

    // Template method for spawning InteractiveObjects
    template <typename T>
    void spawnInteractiveObject();

    // Schedule collectible respawn after a delay
    void scheduleCollectibleRespawn();

    // Getter for gameObjects
    std::vector<std::unique_ptr<GameObject>>& getGameObjects() {
        return gameObjects; // Allows writable access
    }

    // Read-only getter for gameObjects
    const std::vector<std::unique_ptr<GameObject>>& getGameObjects() const {
        return gameObjects; // Provides read-only access
    }
    // Getter for activePowerUps
    std::vector<std::unique_ptr<PowerUpBase>>& getActivePowerUps() {
        return activePowerUps;
    }

    const std::unique_ptr<DepositZone>& getDepositZone() const { return depositZone; }

    // Score management
    void addScore(int points); // Adds points to score (general purpose)
    int getScore() const { return score; } // Getter for score

    bool isPowerUpUpgrading(PowerUpBase* powerup) const; // Check if a power-up has an active upgrade timer

    // Tally management
    void incrementTally(int count); // Increments tally (only deposits)
    int getTally() const { return tally; } // Getter for tally
    void resetTally(); // Resets tally for power-ups

    void resetKillChain();     // Resets the kill chain
    void addToKillChain();     // Increments the kill chain and calculates score

    // Power-Up Management
    void spawnPowerUp(int level);       // Spawns a power-up of a specific level
    void updatePowerUpTimers(float dt); // Handles auto-upgrading of power-ups

    // Getter for upgradeTimers
    std::vector<std::pair<size_t, float>>& getUpgradeTimers() {
        return upgradeTimers;
    }

    bool isAnyPowerUpActive() const; // Checks if any power-up effect is active

    // new shit hope it works
    void markPowerUpForRemoval(PowerUpBase* powerUp);
    void cleanupMarkedPowerUps();
    bool hasPendingRemovals() const { return isPowerUpRemovalPending; }
    void setProcessingUpdates(bool processing) { isProcessingUpdates = processing; }

    // Pause management
    void setPaused(bool paused);         // Set the game to paused or unpaused
    bool isPaused() const;               // Check if the game is paused
    void updatePauseMenu();   // Handles navigation and selection for the pause menu
    void drawPauseMenu();     // Renders the pause menu

    // Pre-game pause ("READY?" state)
    void setPreGamePause(bool preGame);  // Enable or disable the pre-game pause
    bool isPreGamePaused() const;        // Check if the game is in the pre-game pause

    // Reset states for new game
    void resetGameStates();              // Reset all flags to their default state

    bool enemyKilled = false;
    float enemyKillEndTime = 0.0f;

    void incrementMultiplier(); // Increase multiplier
    int getMultiplier() const;  // Get the current multiplier

    // Destructor
    ~GameState();
};

// Template
template <typename T>
void GameState::spawnInteractiveObject() {
    if (isGameOver) return;

    int gridX, gridY;
    bool positionValid = false;
    int attempts = 0;

    while (!positionValid && attempts < 100) {
        // Adjust spawn range to the playable area
        gridX = 1 + (rand() % PLAYABLE_COLUMNS); // Shift by 1 for left column
        gridY = UI_ROWS_ABOVE + (rand() % PLAYABLE_ROWS); // Shift by UI_ROWS_ABOVE for top rows

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

        // Prevent spawning on player's tail segments and adjacent tiles
        Player* player = nullptr;
        for (const auto& obj : gameObjects) {
            player = dynamic_cast<Player*>(obj.get());
            if (player) break;
        }

        if (player) {
            int playerX = player->getGridX();
            int playerY = player->getGridY();

            // Check against player's position and adjacent tiles
            if ((gridX == playerX && gridY == playerY) || // Player's position
                (gridX == playerX - 1 && gridY == playerY) || // Left
                (gridX == playerX + 1 && gridY == playerY) || // Right
                (gridX == playerX && gridY == playerY - 1) || // Up
                (gridX == playerX && gridY == playerY + 1)) { // Down
                positionValid = false;
            }

            // Check against player's tail segments
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
    if constexpr (std::is_same<T, MovingEnemy>::value || std::is_same<T, StationaryEnemy>::value) {
        auto* enemy = new T(this, gridX, gridY);
        enemy->setInactive(4000.0f); // Set inactive state for 4 seconds
        addObject(enemy);
        std::cout << typeid(T).name() << " spawned INACTIVE at (" << gridX << ", " << gridY << ")" << std::endl;
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
    else if constexpr (std::is_same<T, PowerUpLevel4>::value) { // Level 4 Power-Up
        addObject(new PowerUpLevel4(this, gridX, gridY));
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
