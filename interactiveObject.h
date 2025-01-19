#pragma once

#include "gameobject.h"
#include "player.h"

class InteractiveObject : public GameObject {
protected:
    int gridX;
    int gridY;

public:
    InteractiveObject(GameState* state, int x, int y, const std::string& name = "");
    virtual void handleCollision(Player& player); 
    bool checkCollision(const Player& player) const; 
    int getGridX() const { return gridX; }
    int getGridY() const { return gridY; }
    void setPosition(int x, int y) { gridX = x; gridY = y; }
};
