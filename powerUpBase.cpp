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

    graphics::playSound(ASSET_PATH + "sounds/pwrpickup.wav", 0.7f, false);


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

void PowerUpBase::update(float dt) {
    if (isWeakEffectActive) {
        updateWeakEffect(dt); // Continue updating the weak effect timer
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
    if (!isWeakEffectActive || isInCleanupProcess() || isMarkedForRemoval()) {
        return; // Prevent recursive or duplicate cleanup
    }

    isWeakEffectActive = false;

    // Restore enemies
    auto& objects = state->getGameObjects();
    for (const auto& obj : objects) {
        if (!obj || !obj->isActive()) continue;

        // Restore MovingEnemies
        if (auto* movingEnemy = dynamic_cast<MovingEnemy*>(obj.get())) {
            movingEnemy->setWeak(false);
            movingEnemy->startMovement();
            std::cout << "MovingEnemy restored at ("
                << movingEnemy->getGridX() << ", "
                << movingEnemy->getGridY() << ")" << std::endl;
        }

        // Restore StationaryEnemies (for level >= 2)
        if (level >= 2) {
            if (auto* stationaryEnemy = dynamic_cast<StationaryEnemy*>(obj.get())) {
                stationaryEnemy->setWeak(false);
                std::cout << "StationaryEnemy restored at ("
                    << stationaryEnemy->getGridX() << ", "
                    << stationaryEnemy->getGridY() << ")" << std::endl;
            }
        }
    }

    // Mark for removal if not upgrading
    if (!state->isPowerUpUpgrading(this)) {
        setActive(false);
        markForRemoval();
        state->markPowerUpForRemoval(this);
        std::cout << "Power-up marked for removal and deactivated." << std::endl;
    }
}

// Draw (for override)
void PowerUpBase::draw() {
}

// Default init
void PowerUpBase::init() {
    std::cout << "PowerUpBase initialized at Level " << level << std::endl;
}
