#pragma once

#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "GameObject.h"
#include "Collectible.h"

class GameState {
private:
    static GameState* instance;
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    float collectibleSpawnTimer = 0.0f; // Timer for spawning collectibles
    const float collectibleSpawnInterval = 5.0f; // 5 seconds interval
    bool firstSpawnTriggered = false; // Tracks whether the first collectible has spawned

    GameState() {}

public:
    static GameState* getInstance();
    void addObject(GameObject* obj);
    void update(float dt);
    void draw();
    void init();
    void reset();
    void spawnCollectible(); // Spawns a new collectible
    ~GameState();
};

#include "GameState.h"
#include <iostream> // For debug prints

GameState* GameState::instance = nullptr;

GameState* GameState::getInstance() {
    if (!instance) {
        instance = new GameState();
        std::srand(std::time(nullptr)); // Seed random number generator
    }
    return instance;
}

void GameState::addObject(GameObject* obj) {
    gameObjects.emplace_back(obj);
}

void GameState::update(float dt) {
    // Update collectible spawn timer
    collectibleSpawnTimer += dt;

    // Handle the first collectible spawn
    if (!firstSpawnTriggered && collectibleSpawnTimer >= collectibleSpawnInterval) {
        spawnCollectible();
        firstSpawnTriggered = true;  // Mark first spawn as complete
        collectibleSpawnTimer = 0.0f; // Reset timer for subsequent spawns
    }

    // Handle subsequent spawns every 5 seconds
    if (firstSpawnTriggered && collectibleSpawnTimer >= collectibleSpawnInterval) {
        bool hasActiveCollectible = false;

        // Check if an active collectible already exists
        for (const auto& obj : gameObjects) {
            Collectible* collectible = dynamic_cast<Collectible*>(obj.get());
            if (collectible && collectible->isActive()) {
                hasActiveCollectible = true;
                break;
            }
        }

        // Spawn a new collectible only if no active one exists
        if (!hasActiveCollectible) {
            spawnCollectible();
            collectibleSpawnTimer = 0.0f; // Reset timer for the next spawn
        }
    }

    // Update active game objects
    for (auto& obj : gameObjects) {
        if (obj->isActive()) {
            obj->update(dt);
        }
    }
}

void GameState::draw() {
    for (auto& obj : gameObjects) {
        if (obj->isActive()) {
            obj->draw();
        }
    }
}

void GameState::init() {
    collectibleSpawnTimer = 0.0f;
    firstSpawnTriggered = false; // Ensure no collectible spawns at game start
}

void GameState::reset() {
    gameObjects.clear();
    collectibleSpawnTimer = 0.0f;
    firstSpawnTriggered = false;
}

void GameState::spawnCollectible() {
    // Generate random grid position
    int gridX, gridY;
    bool positionValid = false;

    while (!positionValid) {
        gridX = rand() % 12;
        gridY = rand() % 12;
        positionValid = true; // Assume valid initially

        // Ensure no duplicate position
        for (const auto& obj : gameObjects) {
            Collectible* collectible = dynamic_cast<Collectible*>(obj.get());
            if (collectible && collectible->isActive() &&
                collectible->getGridX() == gridX && collectible->getGridY() == gridY) {
                positionValid = false; // Position already occupied
                break;
            }
        }
    }

    // Create and add a new collectible
    Collectible* collectible = new Collectible(this, gridX, gridY);
    addObject(collectible);

    // Debug print for spawn information
    std::cout << "Collectible spawned at: (" << gridX << ", " << gridY << ")" << std::endl;
}

GameState::~GameState() {
    gameObjects.clear();
}
