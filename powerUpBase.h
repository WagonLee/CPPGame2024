#include "PowerUpBase.h"
#include <iostream>

// Constructor
PowerUpBase::PowerUpBase(GameState* state, int x, int y, int level)
    : InteractiveObject(state, x, y, "PowerUp"), level(level) {}

// Placeholder effect (to be implemented later)
void PowerUpBase::applyEffect() {
    std::cout << "Power-up effect triggered for level " << level << std::endl;
}
