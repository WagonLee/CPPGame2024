#pragma once

#include "PowerUpBase.h"

class PowerUpLevel3 : public PowerUpBase {
public:
    PowerUpLevel3(GameState* state, int x, int y);

    void applyEffect() override; // Level 3 effect
    void draw() override;        // Level 3 visual
};
