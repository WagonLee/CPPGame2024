#include "interactiveobject.h"

InteractiveObject::InteractiveObject(GameState* state, int x, int y, const std::string& name)
    : GameObject(state, name), gridX(x), gridY(y) {}

bool InteractiveObject::checkCollision(const Player& player) const {
    return player.getGridX() == gridX && player.getGridY() == gridY;
}

void InteractiveObject::handleCollision(Player& player) {
    setActive(false); 
}
