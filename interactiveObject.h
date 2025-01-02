#pragma once

#include "GameObject.h"
#include "Player.h"

class InteractiveObject : public GameObject {
protected:
    int gridX;
    int gridY;

public:
    InteractiveObject(GameState* state, int x, int y, const std::string& name = "");
    virtual void handleCollision(Player& player); // Made virtual
    bool checkCollision(const Player& player) const; // Check for collision
    int getGridX() const { return gridX; }
    int getGridY() const { return gridY; }
    void setPosition(int x, int y) { gridX = x; gridY = y; }
};
