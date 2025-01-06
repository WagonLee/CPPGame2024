#pragma once

#include "PowerUpBase.h"

// Level 1 Power-Up class
class PowerUpLevel1 : public PowerUpBase {
public:
    // Constructor
    PowerUpLevel1(GameState* state, int x, int y);

    // Apply effect (placeholder for now)
    void applyEffect() override;
};
