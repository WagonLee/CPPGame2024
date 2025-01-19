#include "poweruplevel2.h"
#include "graphics.h"
#include <iostream>
#include "config.h"

PowerUpLevel2::PowerUpLevel2(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 2) {}

void PowerUpLevel2::applyEffect() {

    startWeakEffect(9000.0f); 
}

void PowerUpLevel2::draw() {
    if (!visible) return;

    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.texture = ASSET_PATH + "objects/POWERUP2.png";

    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    graphics::drawRect(xPos, yPos, CELL_SIZE * 0.95, CELL_SIZE * 0.95, br);
}
