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
#include "PowerUpBlue.h" // Example specialized PowerUp

class GameState {
private:
    static GameState* instance;
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // Enemy spawn timing
    time_t lastSpawnTime;
    const double enemySpawnInterval = 2.0;

    // Collectible management
    const int collectibleCount = 2;

    // PowerUp spawn timing
    time_t lastPowerUpSpawnTime;
    const double powerUpSpawnInterval = 5.0;

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

    // Template method for spawning InteractiveObjects
    template <typename T>
    void spawnInteractiveObject();

    // Destructor
    ~GameState();
};

// Template implementation must be in the header file
template <typename T>
void GameState::spawnInteractiveObject() {
    int gridX, gridY;
    bool positionValid = false;
    int attempts = 0;

    while (!positionValid && attempts < 100) {
        gridX = rand() % 12;
        gridY = rand() % 12;
        positionValid = true;

        // Ensure no overlap with any existing InteractiveObject
        for (const auto& obj : gameObjects) {
            InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
            if (interactive && interactive->isActive() &&
                interactive->getGridX() == gridX && interactive->getGridY() == gridY) {
                std::cout << "Conflict detected at: (" << gridX << ", " << gridY << ")" << std::endl;
                positionValid = false; // Retry if conflict
                break;
            }
        }
        attempts++;
    }

    if (!positionValid) {
        std::cerr << "Failed to find valid spawn position!" << std::endl;
        return;
    }

    // Create and add the new InteractiveObject
    T* obj = new T(this, gridX, gridY); // Uses template type T (Enemy, Collectible, or PowerUp)
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
