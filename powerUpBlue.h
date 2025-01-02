#pragma once

#include "PowerUp.h"

class PowerUpBlue : public PowerUp {
public:
    PowerUpBlue(GameState* state, int x, int y);
    void draw() override;
    void update(float dt) override;
    void init() override;
    void applyEffect() override; // Implements specific effect for blue power-up
};
