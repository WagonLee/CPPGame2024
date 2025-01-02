#include "InteractiveObject.h"

// Constructor
InteractiveObject::InteractiveObject(GameState* state, int x, int y, const std::string& name)
    : GameObject(state, name), gridX(x), gridY(y) {}

// Check for collision with the player
bool InteractiveObject::checkCollision(const Player& player) const {
    return player.getGridX() == gridX && player.getGridY() == gridY;
}

// Default collision behavior
void InteractiveObject::handleCollision(Player& player) {
    setActive(false); // Deactivate object by default
}
