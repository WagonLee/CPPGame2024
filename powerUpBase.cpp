#include "PowerUpBase.h"
#include "GameState.h"
#include "graphics.h"
#include <iostream>
#include "MovingEnemy.h"
#include "StationaryEnemy.h"

// Constructor
PowerUpBase::PowerUpBase(GameState* state, int x, int y, int level)
    : InteractiveObject(state, x, y, "PowerUp"), state(state), level(level) {}

// Handle collision
void PowerUpBase::handleCollision(Player& player) {
    if (!canCollide()) return; // Prevent duplicate collisions

    std::cout << "PowerUp Level " << level << " collected!" << std::endl;

    // Mark as collected
    isCollectible = false;
    visible = false;        // Hide visually but keep active for effect
    setActive(true);       // Remains active for timers

    // Apply the effect
    applyEffect();
}


// Weak Effect: Start Timer
void PowerUpBase::startWeakEffect(float duration) {
    weakEffectDuration = duration;
    isWeakEffectActive = true;

    std::cout << "Weak effect started for " << duration << " seconds." << std::endl;

    // Apply effect to MovingEnemies and StationaryEnemies
    for (auto& obj : state->getGameObjects()) {
        MovingEnemy* movingEnemy = dynamic_cast<MovingEnemy*>(obj.get());
        if (movingEnemy && movingEnemy->isActive()) {
            movingEnemy->setWeak(true);
            movingEnemy->stopMovement();
            std::cout << "MovingEnemy weakened at (" << movingEnemy->getGridX()
                << ", " << movingEnemy->getGridY() << ")" << std::endl;
        }

        if (level >= 2) {
            StationaryEnemy* stationaryEnemy = dynamic_cast<StationaryEnemy*>(obj.get());
            if (stationaryEnemy && stationaryEnemy->isActive()) {
                stationaryEnemy->setWeak(true);
                std::cout << "StationaryEnemy weakened at (" << stationaryEnemy->getGridX()
                    << ", " << stationaryEnemy->getGridY() << ")" << std::endl;
            }
        }
    }
}

// Weak Effect: Update Timer
void PowerUpBase::updateWeakEffect(float dt) {
    if (!isWeakEffectActive) return; // Skip if not active

    // Clamp timer to prevent negative values
    weakEffectDuration = std::max(0.0f, weakEffectDuration - dt);

    // Log remaining time
    std::cout << "Weak effect time left: " << weakEffectDuration << " seconds." << std::endl;

    if (weakEffectDuration <= 0.0f) {
        endWeakEffect(); // End the effect
    }
}

// Weak Effect: End Effect
void PowerUpBase::endWeakEffect() {
    isWeakEffectActive = false;

    // Restore MovingEnemies
    for (auto& obj : state->getGameObjects()) {
        MovingEnemy* movingEnemy = dynamic_cast<MovingEnemy*>(obj.get());
        if (movingEnemy && movingEnemy->isActive()) {
            movingEnemy->setWeak(false);
            movingEnemy->startMovement();
            std::cout << "MovingEnemy restored at (" << movingEnemy->getGridX()
                << ", " << movingEnemy->getGridY() << ")" << std::endl;
        }

        if (level >= 2) {
            StationaryEnemy* stationaryEnemy = dynamic_cast<StationaryEnemy*>(obj.get());
            if (stationaryEnemy && stationaryEnemy->isActive()) {
                stationaryEnemy->setWeak(false);
                std::cout << "StationaryEnemy restored at (" << stationaryEnemy->getGridX()
                    << ", " << stationaryEnemy->getGridY() << ")" << std::endl;
            }
        }
    }

    // Fully deactivate only if not upgrading
    if (!state->isPowerUpUpgrading(this)) {
        setActive(false);
        std::cout << "Power-up Level " << level << " fully deactivated." << std::endl;
    }
}

// Update (keeps timer running)
void PowerUpBase::update(float dt) {
    if (isWeakEffectActive) {
        updateWeakEffect(dt); // Update timer and effects
    }
}

// Draw (for override)
void PowerUpBase::draw() {
}

// Default init
void PowerUpBase::init() {
    std::cout << "PowerUpBase initialized at Level " << level << std::endl;
}
