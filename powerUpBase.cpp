#include "PowerUpBase.h"
#include "GameState.h" // Include here for full definition
#include "graphics.h"
#include <iostream>
#include <algorithm> // Required for std::remove_if
#include "MovingEnemy.h" // For handling enemies during weak effect

// Constructor
PowerUpBase::PowerUpBase(GameState* state, int x, int y, int level)
    : InteractiveObject(state, x, y, "PowerUp"), state(state), level(level) {} // Initialize state and level

// Handle collision
void PowerUpBase::handleCollision(Player& player) {
    std::cout << "PowerUp Level " << level << " collected!" << std::endl;
    setActive(false); // Deactivate Power-Up

    // Apply the effect when collected
    applyEffect(); // CALL THIS!

    // Remove the power-up timer
    auto& timers = state->getUpgradeTimers();
    auto& activePowerUps = state->getActivePowerUps();
    timers.erase(
        std::remove_if(timers.begin(), timers.end(),
            [this, &activePowerUps](const std::pair<size_t, float>& t) {
                size_t index = t.first;
    return index >= activePowerUps.size() || activePowerUps[index].get() == this;
            }),
        timers.end());

    std::cout << "Timer removed for collected Power-Up Level " << level << std::endl;
}


// Weak Effect: Start Timer
void PowerUpBase::startWeakEffect(float duration) {
    weakEffectDuration = duration;
    isWeakEffectActive = true;

    std::cout << "Weak effect started for " << duration << " seconds." << std::endl;

    // Apply effect to all MovingEnemies
    for (auto& obj : state->getGameObjects()) {
        MovingEnemy* enemy = dynamic_cast<MovingEnemy*>(obj.get());
        if (enemy && enemy->isActive()) {
            enemy->setWeak(true);    // Weaken the enemy
            enemy->stopMovement();  // Stop its movement
            std::cout << "Enemy at (" << enemy->getGridX() << ", "
                << enemy->getGridY() << ") is now weak!" << std::endl;
        }
    }
}

// Weak Effect: Update Timer
void PowerUpBase::updateWeakEffect(float dt) {
    if (!isWeakEffectActive) return; // Skip if effect is not active

    weakEffectDuration -= dt / 60.0f; // Adjust based on frame rate

    if (weakEffectDuration <= 0.0f) {
        std::cout << "Weak effect ended! Restoring enemy states." << std::endl;
        endWeakEffect(); // Reset enemies when the timer expires
    }
}

// Weak Effect: End Effect
void PowerUpBase::endWeakEffect() {
    isWeakEffectActive = false;

    // Restore all MovingEnemies
    for (auto& obj : state->getGameObjects()) {
        MovingEnemy* enemy = dynamic_cast<MovingEnemy*>(obj.get());
        if (enemy && enemy->isActive()) {
            enemy->setWeak(false);     // Restore strength
            enemy->startMovement();   // Enable movement again
            std::cout << "Enemy at (" << enemy->getGridX() << ", "
                << enemy->getGridY() << ") restored!" << std::endl;
        }
    }
}

// Dummy update() (to satisfy GameObject requirement)
void PowerUpBase::update(float dt) {
    updateWeakEffect(dt); // Keep updating weak effect timer
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
