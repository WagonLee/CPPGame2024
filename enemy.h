#pragma once

#include "InteractiveObject.h"

class Enemy : public InteractiveObject {
public:
    Enemy(GameState* state, int x, int y);
    void draw() override;
    void update(float dt) override;
    void init() override;
    void handleCollision() override; // Placeholder for collision behavior
};
