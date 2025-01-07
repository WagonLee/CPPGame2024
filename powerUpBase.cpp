#include "PowerUpBase.h"
#include "GameState.h" // Include here for full definition
#include "graphics.h"
#include <iostream>
#include <algorithm> // Required for std::remove_if

// Constructor
PowerUpBase::PowerUpBase(GameState* state, int x, int y, int level)
    : InteractiveObject(state, x, y, "PowerUp"), state(state), level(level) {} // Initialize state and level

// Handle collision
void PowerUpBase::handleCollision(Player& player) {
    std::cout << "PowerUp Level " << level << " collected!" << std::endl;

    setActive(false); // Deactivate Power-Up

    // Access upgradeTimers from GameState through 'state'
    auto& timers = state->getUpgradeTimers(); // Access upgrade timers
    auto& activePowerUps = state->getActivePowerUps(); // Access active Power-Ups

    timers.erase(
        std::remove_if(timers.begin(), timers.end(),
            [this, &activePowerUps](const std::pair<size_t, float>& t) { // Use accessors
                size_t index = t.first;
    return index >= activePowerUps.size() || activePowerUps[index].get() == this;
            }),
        timers.end());


    std::cout << "Timer removed for collected Power-Up Level " << level << std::endl;
}

// Dummy update() (to satisfy GameObject requirement)
void PowerUpBase::update(float dt) {
    // No behavior needed; satisfies inheritance requirement
}

// Default draw (should be overridden by subclasses)
void PowerUpBase::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Default white square
    br.fill_color[0] = 1.0f; // White color
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 1.0f;

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}

// Default init() implementation
void PowerUpBase::init() {
    std::cout << "PowerUpBase initialized at Level " << level << std::endl;
}
