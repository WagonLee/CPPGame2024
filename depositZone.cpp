#include "DepositZone.h"
#include "graphics.h"
#include <iostream>
#include "config.h"

// Constructor
DepositZone::DepositZone(GameState* state, int x, int y, Shape shape, bool horizontal)
    : GameObject(state, "DepositZone"), gridX(x + 1), gridY(y + UI_ROWS_ABOVE), shape(shape), horizontal(horizontal) {
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
    }
}

// Create a straight line (horizontal or vertical)
void DepositZone::createStraightLine() {
    if (horizontal) { // Horizontal line
        for (int x = 1; x < 13; ++x) { // Start from column 1 to align properly
            tiles.emplace_back(x, gridY);
        }
    }
    else { // Vertical line
        for (int y = UI_ROWS_ABOVE + 0; y < UI_ROWS_ABOVE + 12; ++y) { // Start from row UI_ROWS_ABOVE + 1 to align
            tiles.emplace_back(gridX, y);
        }
    }
}

// Create a donut shape
void DepositZone::createDonut() {
    // Ensure the shape fits within the playable grid
    int adjustedGridX = std::min(gridX, 1 + PLAYABLE_COLUMNS - 4); // Adjust for right bound
    int adjustedGridY = std::min(gridY, UI_ROWS_ABOVE + PLAYABLE_ROWS - 4); // Adjust for bottom bound

    for (int x = adjustedGridX; x < adjustedGridX + 4; ++x) {
        for (int y = adjustedGridY; y < adjustedGridY + 4; ++y) {
            if ((x == adjustedGridX || x == adjustedGridX + 3) || (y == adjustedGridY || y == adjustedGridY + 3)) {
                tiles.emplace_back(x, y);
            }
        }
    }
}

// Create a circle shape
void DepositZone::createCircle() {
    // Ensure the shape fits within the playable grid
    int adjustedGridX = std::min(gridX, 1 + PLAYABLE_COLUMNS - 4); // Adjust for right bound
    int adjustedGridY = std::min(gridY, UI_ROWS_ABOVE + PLAYABLE_ROWS - 4); // Adjust for bottom bound

    for (int x = adjustedGridX; x < adjustedGridX + 4; ++x) {
        for (int y = adjustedGridY; y < adjustedGridY + 4; ++y) {
            // Exclude the corners
            if (!((x == adjustedGridX && y == adjustedGridY) ||                 // Top-left corner
                (x == adjustedGridX + 3 && y == adjustedGridY) ||             // Top-right corner
                (x == adjustedGridX && y == adjustedGridY + 3) ||             // Bottom-left corner
                (x == adjustedGridX + 3 && y == adjustedGridY + 3))) {        // Bottom-right corner
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