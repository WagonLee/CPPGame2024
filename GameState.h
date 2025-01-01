#pragma once

#include <vector>
#include <memory>
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
