#pragma once

#include "GameObject.h"
#include "graphics.h"

class Collectible : public GameObject {
private:
    float x, y;         // Position in pixels
    int gridX, gridY;   // Grid position
    bool active;        // Is the collectible active?

public:
    Collectible(GameState* gs, int gridX, int gridY);
    void update(float dt) override;
    void draw() override;
    void init() override;

    bool isActive() const { return active; }
    void setActive(bool status) { active = status; }

    int getGridX() const { return gridX; }
    int getGridY() const { return gridY; }
};
