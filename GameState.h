#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <memory>
#include "GameObject.h"

class GameState {
private:
    static GameState* instance;
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    GameState() {}

public:
    static GameState* getInstance();
    void addObject(GameObject* obj);
    void update(float dt);
    void draw();
    void init();
    void reset();
    ~GameState();
};

#endif // GAMESTATE_H
