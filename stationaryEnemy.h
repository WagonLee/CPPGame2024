#pragma once

#include "enemy.h"
#include "graphics.h"

class StationaryEnemy : public Enemy {
public:
    StationaryEnemy(GameState* state, int x, int y);

    // Override methods
    void draw() override;       
    void update(float dt) override; 
    void init() override;      
};