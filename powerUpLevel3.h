#pragma once

#include "PowerUpBase.h"

// Level 3 Power-Up class
class PowerUpLevel3 : public PowerUpBase {
public:
    // Constructor
    PowerUpLevel3(GameState* state, int x, int y);

    // Apply effect (placeholder for now)
    void applyEffect() override;
};
