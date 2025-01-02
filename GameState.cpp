#include "GameState.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

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
    if (isGameOver) return; // Stop all updates after player death

    // ENEMY SPAWNING
    time_t currentTime = time(nullptr);
    if (difftime(currentTime, lastSpawnTime) >= enemySpawnInterval) {
        spawnInteractiveObject<Enemy>();
        lastSpawnTime = currentTime;
        enemySpawnInterval = enemySpawnMin + (rand() / (RAND_MAX / (enemySpawnMax - enemySpawnMin)));
    }

    // COLLECTIBLE MANAGEMENT
    int activeCollectibles = 0;
    for (const auto& obj : gameObjects) {
        Collectible* collectible = dynamic_cast<Collectible*>(obj.get());
        if (collectible && collectible->isActive()) {
            activeCollectibles++;
        }
    }

    while (activeCollectibles < collectibleCount) {
        spawnInteractiveObject<Collectible>();
        activeCollectibles++;
    }

    // POWERUP SPAWNING
    if (difftime(currentTime, lastPowerUpSpawnTime) >= powerUpSpawnInterval) {
        spawnInteractiveObject<PowerUpBlue>();
        lastPowerUpSpawnTime = currentTime;
        powerUpSpawnInterval = powerUpSpawnMin + (rand() / (RAND_MAX / (powerUpSpawnMax - powerUpSpawnMin)));
    }

    // UPDATE ALL OBJECTS
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
    lastSpawnTime = time(nullptr);
    lastPowerUpSpawnTime = time(nullptr);
    srand(static_cast<unsigned int>(time(nullptr)));

    enemySpawnInterval = enemySpawnMin + (rand() / (RAND_MAX / (enemySpawnMax - enemySpawnMin)));
    powerUpSpawnInterval = powerUpSpawnMin + (rand() / (RAND_MAX / (powerUpSpawnMax - powerUpSpawnMin)));

    for (int i = 0; i < collectibleCount; i++) {
        spawnInteractiveObject<Collectible>();
    }
}

void GameState::reset() {}

// Handles stopping activity after player death
void GameState::endGame() {
    isGameOver = true;
    std::cout << "Game Over! All activity stopped." << std::endl;
}

GameState::~GameState() {
    gameObjects.clear();
}
