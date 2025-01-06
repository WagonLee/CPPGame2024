#pragma once

#include "PowerUpBase.h"

// Level 2 Power-Up class
class PowerUpLevel2 : public PowerUpBase {
public:
    // Constructor
    PowerUpLevel2(GameState* state, int x, int y);

    // Apply effect (placeholder for now)
    void applyEffect() override;
};
