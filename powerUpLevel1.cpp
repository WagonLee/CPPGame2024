#include "poweruplevel1.h"
#include "graphics.h"
#include <iostream>
#include "config.h"

PowerUpLevel1::PowerUpLevel1(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 1) {}

void PowerUpLevel1::applyEffect() {
    std::cout << "Level 1 Power-Up effect applied! Weakening MovingEnemies for 10 seconds." << std::endl;

    startWeakEffect(9000.0f);
}

void PowerUpLevel1::draw() {
    if (!visible) return;

    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.texture = ASSET_PATH + "objects/POWERUP1.png";

    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    graphics::drawRect(xPos, yPos, CELL_SIZE * 0.95, CELL_SIZE * 0.95, br);
}
