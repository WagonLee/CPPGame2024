#include "DepositZone.h"
#include "graphics.h"
#include <iostream>

const float CELL_SIZE = 50.0f; // Match grid cell size

// Constructor
DepositZone::DepositZone(GameState* state, int x, int y, Shape shape, bool horizontal)
    : GameObject(state, "DepositZone"), gridX(x), gridY(y), shape(shape), horizontal(horizontal) {
    duration = 185.0f;  // 10 seconds active time
    timer = duration;
    init(); // Calculate zone tiles
}

// Initialize zone shape
void DepositZone::init() {
    tiles.clear();

    switch (shape) {
    case Shape::STRAIGHT_LINE:
        createStraightLine();
        break;
    case Shape::DONUT:
        createDonut();
        break;
    case Shape::CIRCLE:
        createCircle();
        break;
    default:
        std::cerr << "Unknown shape type!" << std::endl;
    }
}

// Create a straight line (horizontal or vertical)
void DepositZone::createStraightLine() {
    if (horizontal) { // Horizontal line
        for (int x = 0; x < 12; ++x) {
            tiles.emplace_back(x, gridY);
        }
    }
    else { // Vertical line
        for (int y = 0; y < 12; ++y) {
            tiles.emplace_back(gridX, y);
        }
    }
}

// Create a donut shape
void DepositZone::createDonut() {
    for (int x = gridX; x < gridX + 4; ++x) {
        for (int y = gridY; y < gridY + 4; ++y) {
            if ((x == gridX || x == gridX + 3) || (y == gridY || y == gridY + 3)) {
                tiles.emplace_back(x, y);
            }
        }
    }
}

// Create a circle shape
void DepositZone::createCircle() {
    for (int x = gridX; x < gridX + 4; ++x) {
        for (int y = gridY; y < gridY + 4; ++y) {
            // Exclude the corners
            if (!((x == gridX && y == gridY) ||                 // Top-left corner
                (x == gridX + 3 && y == gridY) ||             // Top-right corner
                (x == gridX && y == gridY + 3) ||             // Bottom-left corner
                (x == gridX + 3 && y == gridY + 3))) {        // Bottom-right corner
                tiles.emplace_back(x, y);
            }
        }
    }
}

// Check if a tile is part of the zone
bool DepositZone::isTileInZone(int x, int y) const {
    for (const auto& tile : tiles) {
        if (tile.first == x && tile.second == y) {
            return true;
        }
    }
    return false;
}

// Update zone timer
void DepositZone::update(float dt) {
    // Clamp dt to avoid large jumps causing instant expiration
    if (dt > 0.1f) dt = 0.1f; // Limit to 100ms/frame

    if (timer > 0) {
        timer -= dt; // Decrement timer based on safe dt
    }
}


// Draw the deposit zone
void DepositZone::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.fill_color[0] = 1.0f;  // Orange
    br.fill_color[1] = 0.5f;
    br.fill_color[2] = 0.0f;

    for (const auto& tile : tiles) {
        float xPos = tile.first * CELL_SIZE + CELL_SIZE / 2.0f;
        float yPos = tile.second * CELL_SIZE + CELL_SIZE / 2.0f;
        graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
    }
}
