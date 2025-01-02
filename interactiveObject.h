#pragma once

#include "GameObject.h"

class InteractiveObject : public GameObject {
protected:
    int gridX;
    int gridY;

public:
    InteractiveObject(GameState* state, int x, int y, const std::string& name = "");
    virtual void handleCollision() = 0; // Pure virtual for collision behavior

    // Shared functionality
    int getGridX() const { return gridX; }
    int getGridY() const { return gridY; }
    void setPosition(int x, int y) { gridX = x; gridY = y; }
};
