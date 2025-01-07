#include "PowerUpLevel3.h"
#include "graphics.h"
#include <iostream>

// Constructor
PowerUpLevel3::PowerUpLevel3(GameState* state, int x, int y)
    : PowerUpBase(state, x, y, 3) {}

// Apply effect
void PowerUpLevel3::applyEffect() {
    std::cout << "Level 3 Power-Up effect applied!" << std::endl;
}

// Draw
void PowerUpLevel3::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Purple for Level 3
    br.fill_color[0] = 0.5f;
    br.fill_color[1] = 0.0f;
    br.fill_color[2] = 0.5f;

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}
