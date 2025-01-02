#pragma once

#include "InteractiveObject.h"

class PowerUp : public InteractiveObject {
public:
    PowerUp(GameState* state, int x, int y, const std::string& name = "PowerUp");
    virtual void applyEffect() = 0; // Pure virtual for specific effects
    void handleCollision(Player& player) override; // Now valid
};
