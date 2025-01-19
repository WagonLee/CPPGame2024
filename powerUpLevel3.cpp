#include "poweruplevel3.h"
#include "graphics.h"
#include <iostream>
#include "gamestate.h"
#include "config.h"

PowerUpLevel3::PowerUpLevel3(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 3) {}

void PowerUpLevel3::applyEffect() {
    startWeakEffect(9000.0f);

    GameState::getInstance()->incrementMultiplier();
    std::cout << "Score multiplier increased due to Level 3 Power-Up!" << std::endl;
}

void PowerUpLevel3::draw() {
    if (!visible) return;

    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.texture = ASSET_PATH + "objects/POWERUP3.png";

    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    graphics::drawRect(xPos, yPos, CELL_SIZE * 0.95, CELL_SIZE * 0.95, br);
}
