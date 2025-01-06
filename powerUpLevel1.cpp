#include "PowerUpLevel1.h"
#include <iostream>

// Constructor
PowerUpLevel1::PowerUpLevel1(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 1) {}

// Placeholder effect
void PowerUpLevel1::applyEffect() {
    std::cout << "Level 1 Power-Up effect applied!" << std::endl;
}
