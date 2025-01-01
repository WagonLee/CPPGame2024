#include "GameState.h"

GameState* GameState::instance = nullptr;

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

void GameState::init() {}
void GameState::reset() {}
GameState::~GameState() { gameObjects.clear(); }
