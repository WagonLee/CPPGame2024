#include "poweruplevel4.h"
#include "graphics.h"
#include <iostream>
#include "gamestate.h" 
#include "config.h"

PowerUpLevel4::PowerUpLevel4(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 4) {}

void PowerUpLevel4::applyEffect() {
    startWeakEffect(9000.0f); 

    std::cout << "Spawning 1 additional collectible as part of Level 4 effect." << std::endl;
    state->spawnInteractiveObject<Collectible>();

    GameState::getInstance()->incrementMultiplier();
    std::cout << "Score multiplier increased due to Level 3 Power-Up!" << std::endl;         
}

// Draw
void PowerUpLevel4::draw() {
    if (!visible) return;

    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.texture = ASSET_PATH + "objects/POWERUP4.png";

    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    graphics::drawRect(xPos, yPos, CELL_SIZE * 0.95 , CELL_SIZE * 0.95 , br);
}
