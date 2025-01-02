#include "GameState.h"
#include <iostream>  // For debug prints
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For random seed and timestamps
#include <chrono>    // For milliseconds precision

GameState* GameState::instance = nullptr;

GameState::GameState() {}

GameState* GameState::getInstance() {
    if (!instance) {
        instance = new GameState();
    }
    return instance;
}

void GameState::addObject(GameObject* obj) {
    gameObjects.emplace_back(obj);
}

void GameState::update(float dt) {
    // --- ENEMY SPAWNING ---
    time_t currentTime = time(nullptr);
    if (difftime(currentTime, lastSpawnTime) >= enemySpawnInterval) {
        spawnInteractiveObject<Enemy>(); // Template-based spawn
        lastSpawnTime = currentTime; // Reset spawn timer
    }

    // --- COLLECTIBLE MANAGEMENT ---
    int activeCollectibles = 0;
    for (const auto& obj : gameObjects) {
        Collectible* collectible = dynamic_cast<Collectible*>(obj.get());
        if (collectible && collectible->isActive()) {
            activeCollectibles++;
        }
    }

    while (activeCollectibles < collectibleCount) {
        spawnInteractiveObject<Collectible>(); // Template-based spawn
        activeCollectibles++;
    }

    // --- POWERUP SPAWNING ---
    if (difftime(currentTime, lastPowerUpSpawnTime) >= powerUpSpawnInterval) {
        spawnInteractiveObject<PowerUpBlue>(); // Example PowerUp
        lastPowerUpSpawnTime = currentTime; // Reset timer
    }

    // --- UPDATE ALL OBJECTS ---
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
    // Reset timers and state
    lastSpawnTime = time(nullptr);
    lastPowerUpSpawnTime = time(nullptr);
    srand(static_cast<unsigned int>(time(nullptr)));

    // Spawn 2 collectibles at game start
    for (int i = 0; i < collectibleCount; i++) {
        spawnInteractiveObject<Collectible>();
    }
}

void GameState::reset() {}

GameState::~GameState() {
    gameObjects.clear();
}
