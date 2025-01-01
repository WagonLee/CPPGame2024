#include "Collectible.h"

const float CELL_SIZE = 50.0f; // Match grid cell size

Collectible::Collectible(GameState* gs, int gridX, int gridY)
    : GameObject(gs, "Collectible"), gridX(gridX), gridY(gridY), active(true) {
    x = gridX * CELL_SIZE + CELL_SIZE / 2;
    y = gridY * CELL_SIZE + CELL_SIZE / 2;
}

void Collectible::init() {
    active = true;
}

void Collectible::update(float dt) {
    // Collectibles do not need to update behavior for now
}

void Collectible::draw() {
    if (active) {
        graphics::Brush brush;
        brush.fill_color[0] = 1.0f; // Yellow color
        brush.fill_color[1] = 1.0f;
        brush.fill_color[2] = 0.0f;
        brush.fill_opacity = 1.0f;

        graphics::drawRect(x, y, CELL_SIZE * 0.6f, CELL_SIZE * 0.6f, brush);
    }
}
