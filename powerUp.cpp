#include "PowerUp.h"
#include <iostream>   // For debug output

// Constructor for base PowerUp
PowerUp::PowerUp(GameState* state, int x, int y, const std::string& name)
    : InteractiveObject(state, x, y, name) {}

// Generic collision handling (can be overridden later)
void PowerUp::handleCollision() {
    std::cout << "PowerUp collected: " << m_name << std::endl;
}
