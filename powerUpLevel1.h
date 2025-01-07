#pragma once

#include "PowerUpBase.h"

class PowerUpLevel1 : public PowerUpBase {
public:
    PowerUpLevel1(GameState* state, int x, int y);

    void applyEffect() override; // Effect implementation
    void draw() override;        // Visual representation
};
