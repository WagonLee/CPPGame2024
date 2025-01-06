#include "PowerUpLevel3.h"
#include <iostream>

// Constructor
PowerUpLevel3::PowerUpLevel3(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 3) {}

// Placeholder effect
void PowerUpLevel3::applyEffect() {
    std::cout << "Level 3 Power-Up effect applied!" << std::endl;
}
