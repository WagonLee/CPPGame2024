#include "PowerUpLevel2.h"
#include <iostream>

// Constructor
PowerUpLevel2::PowerUpLevel2(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 2) {}

// Placeholder effect
void PowerUpLevel2::applyEffect() {
    std::cout << "Level 2 Power-Up effect applied!" << std::endl;
}
