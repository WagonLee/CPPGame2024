#include "PowerUpBlue.h"
#include "graphics.h"
#include <iostream>   // For debug output

// Constructor for blue power-up
PowerUpBlue::PowerUpBlue(GameState* state, int x, int y)
    : PowerUp(state, x, y, "PowerUpBlue") {}

// Draw the blue power-up
void PowerUpBlue::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.fill_color[0] = 0.0f;   // Blue (R)
    br.fill_color[1] = 0.0f;   // Blue (G)
    br.fill_color[2] = 1.0f;   // Blue (B)

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}

// Update behavior (placeholder)
void PowerUpBlue::update(float dt) {}

// Initialize behavior (placeholder)
void PowerUpBlue::init() {}

// Apply effect (to be expanded later)
void PowerUpBlue::applyEffect() {
    std::cout << "PowerUpBlue effect applied!" << std::endl;
}
