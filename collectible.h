#pragma once

#include "InteractiveObject.h"

class Collectible : public InteractiveObject {
public:
    Collectible(GameState* state, int x, int y);
    void draw() override;
    void update(float dt) override;
    void init() override;
    void handleCollision(Player& player) override; // No changes needed
};
