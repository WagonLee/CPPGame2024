#include "PowerUpLevel4.h"
#include "graphics.h"
#include <iostream>
#include "GameState.h" // Required for collectible spawning
#include "config.h"

// Constructor
PowerUpLevel4::PowerUpLevel4(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 4) {}

// Apply effect
void PowerUpLevel4::applyEffect() {
    std::cout << "Level 4 Power-Up effect applied! Weakening all enemies for 20 seconds and spawning 1 collectible." << std::endl;
    graphics::playSound(ASSET_PATH + "pwrpickup.wav", 0.7f, false);

    // Use the centralized weakness logic in PowerUpBase
    startWeakEffect(9000.0f); 

    // Spawn 1 additional collectible immediately
    std::cout << "Spawning 1 additional collectible as part of Level 4 effect." << std::endl;
    state->spawnInteractiveObject<Collectible>();

    // Increment the score multiplier in GameState
    GameState::getInstance()->incrementMultiplier();
    std::cout << "Score multiplier increased due to Level 3 Power-Up!" << std::endl;         
}

// Draw
void PowerUpLevel4::draw() {
    if (!visible) return;

    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.texture = ASSET_PATH + "objects/POWERUP4.png";

    // Use CELL_SIZE for proper alignment
    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    // Draw the power-up with the texture applied
    graphics::drawRect(xPos, yPos, CELL_SIZE * 0.95 , CELL_SIZE * 0.95 , br);
}
