#include "PowerUpLevel1.h"
#include "graphics.h"
#include <iostream>

// Constructor
PowerUpLevel1::PowerUpLevel1(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 1) {}

// Apply effect
void PowerUpLevel1::applyEffect() {
    std::cout << "Level 1 Power-Up effect applied!" << std::endl;
}

// Draw
void PowerUpLevel1::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Brown for Level 1
    br.fill_color[0] = 0.65f;
    br.fill_color[1] = 0.16f;
    br.fill_color[2] = 0.16f;

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}

// Start the Weak Effect Timer
void GameState::startWeakEffectTimer(float duration) {
    weakEffectTimer = duration;
    isWeakEffectActive = true;
    std::cout << "Weak effect timer started for " << duration << " seconds." << std::endl;
}

// Update the Weak Effect Timer
void GameState::updateWeakEffect(float dt) {
    if (!isWeakEffectActive) return; // Skip if the effect is not active

    weakEffectTimer -= dt / 60.0f; // Scale time correctly

    if (weakEffectTimer <= 0.0f) {
        std::cout << "Weak effect ended! Restoring enemy states." << std::endl;

        // Reset all enemies to normal state
        for (auto& obj : gameObjects) {
            MovingEnemy* enemy = dynamic_cast<MovingEnemy*>(obj.get());
            if (enemy && enemy->isActive()) {
                enemy->setWeak(false);  // Restore strength
                enemy->startMovement(); // Allow movement again
                std::cout << "Enemy at (" << enemy->getGridX()
                    << ", " << enemy->getGridY() << ") restored!" << std::endl;
            }
        }

        isWeakEffectActive = false; // Disable the effect
    }
}
