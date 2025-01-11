#pragma once

#include "PowerUpBase.h" // Include the base class explicitly

class PowerUpLevel4 : public PowerUpBase {
public:
    PowerUpLevel4(GameState* state, int x, int y);

    void applyEffect() override; // Level 4 effect
    void draw() override;        // Level 4 visual
};
