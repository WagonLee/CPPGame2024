#include "InteractiveObject.h"

InteractiveObject::InteractiveObject(GameState* state, int x, int y, const std::string& name)
    : GameObject(state, name), gridX(x), gridY(y) {}
