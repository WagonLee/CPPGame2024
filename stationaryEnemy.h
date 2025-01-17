#pragma once

#include "Enemy.h"
#include "graphics.h"

class StationaryEnemy : public Enemy {
public:
    // Constructor
    StationaryEnemy(GameState* state, int x, int y);

    // Override methods
    void draw() override;       // Cyan color for distinction
    void update(float dt) override; // No movement logic needed
    void init() override;       // Initialize behavior
};