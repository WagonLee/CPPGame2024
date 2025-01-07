#include "PowerUpBase.h"
#include "graphics.h"
#include <iostream>

// Constructor
PowerUpBase::PowerUpBase(GameState* state, int x, int y, int level)
    : InteractiveObject(state, x, y, "PowerUp"), level(level) {}

// Handle collision
void PowerUpBase::handleCollision(Player& player) {
    std::cout << "PowerUp Level " << level << " collected!" << std::endl;
    setActive(false); // Deactivate when collected
}

// Dummy update() (to satisfy GameObject)
void PowerUpBase::update(float dt) {
    // No behavior needed, satisfies inheritance requirement
}

// Default draw (should be overridden by subclasses)
void PowerUpBase::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Default white square
    br.fill_color[0] = 1.0f;
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 1.0f;

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}

// Default init()
void PowerUpBase::init() {
    std::cout << "PowerUpBase initialized at Level " << level << std::endl;
}
