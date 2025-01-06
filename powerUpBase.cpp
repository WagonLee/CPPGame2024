#include "PowerUp.h"
#include <iostream>

// Constructor for base PowerUp
PowerUp::PowerUp(GameState* state, int x, int y, const std::string& name)
    : InteractiveObject(state, x, y, name) {}

// Generic collision handling (can be overridden later)
void PowerUp::handleCollision(Player& player) {
    std::cout << "PowerUp collected: " << m_name << std::endl;
    setActive(false); // Default behavior: deactivate upon collision
}
