#pragma once

#include "PowerUpBase.h" // Include the base class explicitly

class PowerUpLevel2 : public PowerUpBase {
public:
    PowerUpLevel2(GameState* state, int x, int y);

    void applyEffect() override; // Level 2 effect
    void draw() override;        // Level 2 visual
};
