#pragma once

#include <vector>
#include <memory>
#include <ctime>      
#include <iostream>   
#include <chrono>    
#include "gameobject.h"
#include "interactiveobject.h"
#include "movingenemy.h"
#include "stationaryenemy.h"
#include "collectible.h"
#include "poweruplevel1.h" 
#include "poweruplevel2.h" 
#include "poweruplevel3.h" 
#include "poweruplevel4.h" 
#include "player.h"
#include "depositzone.h" 
#include "config.h"
#include "gridrenderer.h"

// Forward declare power ups
class PowerUpBase;     
class PowerUpLevel1;    
class PowerUpLevel2;   
class PowerUpLevel3;   
class PowerUpLevel4;    

class GameState {
private:

    GameState();

    static GameState* instance;
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    std::unique_ptr<DepositZone> depositZone; 
    const double depositZoneDuration = 10.0; 

    const int collectibleCount = 2;

    std::vector<std::pair<time_t, time_t>> collectibleRespawnTimers;
    const double collectibleRespawnDelay = 0; 

    bool isGameOver = false;

    std::vector<PowerUpBase*> powerUpsToRemove;
    bool isPowerUpRemovalPending = false;      
    bool isProcessingUpdates = false;          

    void spawnDepositZone();  

    int score = 0;       
    int hiScore = 0; 
    int tally = 0;         

    int killChain = 0;         
    int killChainScore = 0;    
    int scoreMulti = 1;

    // Power-Up Management
    std::vector<std::unique_ptr<PowerUpBase>> activePowerUps; 
    std::vector<std::pair<size_t, float>> upgradeTimers;      

    void spawnPowerUpAt(int level, int gridX, int gridY);

    const int tallyLevel1 = 6; 
    const int tallyLevel2 = 7;
    const int tallyLevel3 = 8;
    const int tallyLevel4 = 9;

    const float upgradeTime = 400.0f; 

    bool preGamePaused = true; 
    bool paused = false;       
    int pauseMenuSelection = 0; 

    bool firstSpawn = true;            
    float firstSpawnTime = 0.0f;        
    bool enemySpawnedInactive = false; 

public:

    std::vector<std::vector<Tile>> deathMenuGridState; 
    int deathMenuSelection = 0;                       

    // Death menu rendering and navigation
    void clearGrid(std::vector<std::vector<Tile>>& grid); 
    void drawTitle(std::vector<std::vector<Tile>>& grid, const std::vector<std::string>& title, int row); 
    void drawOptions(std::vector<std::vector<Tile>>& grid, const std::vector<std::vector<std::string>>& options, int startRow, int selectedIndex); 
    void drawDeathMenu();    
    void updateDeathMenu();  

    bool waitingForDeathMenuInput = false; 

    static GameState* getInstance();

    void addObject(GameObject* obj);
    void update(float dt);
    void draw();
    void init();
    void reset();
    void endGame(); 

    void replaceDepositZone(); 

    // Template method for spawning InteractiveObjects
    template <typename T>
    void spawnInteractiveObject();

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

    // Getter for upgradeTimers
    std::vector<std::pair<size_t, float>>& getUpgradeTimers() {
        return upgradeTimers;
    }

    const std::unique_ptr<DepositZone>& getDepositZone() const { return depositZone; }

    void addScore(int points); 
    int getScore() const { return score; } 

    int getHiScore() const { return hiScore; } 
    void updateHiScore(int score);            

    bool isPowerUpUpgrading(PowerUpBase* powerup) const; 

    void incrementTally(int count); 
    int getTally() const { return tally; } 
    void resetTally(); 

    void resetKillChain();    
    void addToKillChain();    

    void spawnPowerUp(int level);      
    void updatePowerUpTimers(float dt); 


    bool isAnyPowerUpActive() const; 
    void markPowerUpForRemoval(PowerUpBase* powerUp);
    void cleanupMarkedPowerUps();
    bool hasPendingRemovals() const { return isPowerUpRemovalPending; }
    void setProcessingUpdates(bool processing) { isProcessingUpdates = processing; }    

    void setPreGamePause(bool preGame);  
    bool isPreGamePaused() const;        

    void resetGameStates();              

    bool enemyKilled = false;
    float enemyKillEndTime = 0.0f;

    void incrementMultiplier(); 
    int getMultiplier() const;  

    ~GameState();
};

template <typename T>
void GameState::spawnInteractiveObject() {
    if (isGameOver) return;

    int gridX, gridY;
    bool positionValid = false;
    int attempts = 0;

    while (!positionValid && attempts < 100) {
        gridX = 1 + (rand() % PLAYABLE_COLUMNS); // Shift by 1 for left column
        gridY = UI_ROWS_ABOVE + (rand() % PLAYABLE_ROWS); 

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
            if ((gridX == playerX && gridY == playerY) || 
                (gridX == playerX - 1 && gridY == playerY) || 
                (gridX == playerX + 1 && gridY == playerY) || 
                (gridX == playerX && gridY == playerY - 1) || 
                (gridX == playerX && gridY == playerY + 1)) { 
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

    if constexpr (std::is_same<T, MovingEnemy>::value || std::is_same<T, StationaryEnemy>::value) {
        auto* enemy = new T(this, gridX, gridY);
        enemy->setInactive(4000.0f); 
        addObject(enemy);
        std::cout << typeid(T).name() << " spawned INACTIVE at (" << gridX << ", " << gridY << ")" << std::endl;
    }
    else if constexpr (std::is_same<T, Collectible>::value) {
        addObject(new Collectible(this, gridX, gridY));
    }
    else if constexpr (std::is_same<T, PowerUpLevel1>::value) { 
        addObject(new PowerUpLevel1(this, gridX, gridY));
    }
    else if constexpr (std::is_same<T, PowerUpLevel2>::value) { 
        addObject(new PowerUpLevel2(this, gridX, gridY));
    }
    else if constexpr (std::is_same<T, PowerUpLevel3>::value) { 
        addObject(new PowerUpLevel3(this, gridX, gridY));
    }
    else if constexpr (std::is_same<T, PowerUpLevel4>::value) { 
        addObject(new PowerUpLevel4(this, gridX, gridY));
    }

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
