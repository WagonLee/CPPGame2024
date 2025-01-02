#include "GameState.h"
#include <iostream>  // Debug output added
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
    if (isGameOver) return; // Stop updates after player death

    // --- ENEMY SPAWNING ---
    time_t currentTime = time(nullptr);
    if (difftime(currentTime, lastSpawnTime) >= enemySpawnInterval) {
        spawnInteractiveObject<Enemy>();
        lastSpawnTime = currentTime;
        enemySpawnInterval = enemySpawnMin + (rand() / (RAND_MAX / (enemySpawnMax - enemySpawnMin)));
    }

    // --- COLLECTIBLE RESPAWN MANAGEMENT ---
    for (auto it = collectibleRespawnTimers.begin(); it != collectibleRespawnTimers.end();) {
        if (difftime(currentTime, it->second) >= collectibleRespawnDelay) {
            spawnInteractiveObject<Collectible>(); // Respawn collectible
            it = collectibleRespawnTimers.erase(it); // Remove from timer list
            std::cout << "Collectible respawned at: " << currentTime << " seconds.\n";
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
        if (player) break; // Found player instance
    }

    if (player) {
        for (auto& obj : gameObjects) {
            InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
            if (interactive && interactive->isActive()) {
                if (interactive->checkCollision(*player)) {
                    interactive->handleCollision(*player); // Handle collision
                }
            }
        }
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
    lastSpawnTime = time(nullptr);
    lastPowerUpSpawnTime = time(nullptr);
    srand(static_cast<unsigned int>(time(nullptr)));

    enemySpawnInterval = enemySpawnMin + (rand() / (RAND_MAX / (enemySpawnMax - enemySpawnMin)));
    powerUpSpawnInterval = powerUpSpawnMin + (rand() / (RAND_MAX / (powerUpSpawnMax - powerUpSpawnMin)));

    // Spawn initial collectibles
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

void GameState::scheduleCollectibleRespawn() {
    time_t currentTime = time(nullptr);
    collectibleRespawnTimers.emplace_back(currentTime, currentTime + static_cast<time_t>(collectibleRespawnDelay));
    std::cout << "Collectible scheduled for respawn at: " << currentTime + collectibleRespawnDelay << " seconds.\n";
}

GameState::~GameState() {
    gameObjects.clear();
}
