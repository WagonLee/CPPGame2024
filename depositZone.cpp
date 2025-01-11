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
    if (shape == Shape::STRAIGHT_LINE) {
        drawStraightLine();
    }
    else if (shape == Shape::DONUT) {
        drawDonut();
    }
    else if (shape == Shape::CIRCLE) {
        drawCircle();
    }
}

void DepositZone::drawStraightLine() {
    graphics::Brush br;

    for (size_t i = 0; i < tiles.size(); ++i) {
        const auto& tile = tiles[i];
        float xPos = tile.first * CELL_SIZE + CELL_SIZE / 2.0f;
        float yPos = tile.second * CELL_SIZE + CELL_SIZE / 2.0f;

        if (horizontal) {
            // Horizontal straight line
            if (i == 0) {
                // Left edge
                br.fill_color[0] = 0.2f; // Dark green
                br.fill_color[1] = 0.1f;
                br.fill_color[2] = 0.7f;
                br.outline_opacity = 1.0f;
            }
            else if (i == tiles.size() - 1) {
                // Right edge
                br.fill_color[0] = 0.2f; // Dark green
                br.fill_color[1] = 0.5f;
                br.fill_color[2] = 0.0f;
                br.outline_opacity = 1.0f;
            }
            else {
                // Middle tiles
                br.fill_color[0] = 0.0f; // Dark green
                br.fill_color[1] = 1.0f;
                br.fill_color[2] = 0.0f;
                br.outline_opacity = 0.5f; // Semi-transparent outline
            }
        }
        else {
            // Vertical straight line
            if (i == 0) {
                // Top edge
                br.fill_color[0] = 0.0f; // Dark green
                br.fill_color[1] = 0.5f;
                br.fill_color[2] = 0.0f;
                br.outline_opacity = 1.0f;
            }
            else if (i == tiles.size() - 1) {
                // Bottom edge
                br.fill_color[0] = 0.1f; // Dark green
                br.fill_color[1] = 0.5f;
                br.fill_color[2] = 1.0f;
                br.outline_opacity = 1.0f;
            }
            else {
                // Middle tiles
                br.fill_color[0] = 1.0f; // Dark green
                br.fill_color[1] = 0.5f;
                br.fill_color[2] = 1.0f;
                br.outline_opacity = 0.5f; // Semi-transparent outline
            }
        }

        // Draw the tile
        graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
    }
}

void DepositZone::drawDonut() {
    graphics::Brush br;

    for (const auto& tile : tiles) {
        float xPos = tile.first * CELL_SIZE + CELL_SIZE / 2.0f;
        float yPos = tile.second * CELL_SIZE + CELL_SIZE / 2.0f;

        // Determine the position of the tile relative to the 4x4 donut
        bool isTopLeftCorner = (tile.first == gridX && tile.second == gridY);                // Top-left corner
        bool isTopRightCorner = (tile.first == gridX + 3 && tile.second == gridY);           // Top-right corner
        bool isBottomLeftCorner = (tile.first == gridX && tile.second == gridY + 3);         // Bottom-left corner
        bool isBottomRightCorner = (tile.first == gridX + 3 && tile.second == gridY + 3);    // Bottom-right corner

        bool isHorizontalEdge = (tile.second == gridY || tile.second == gridY + 3) &&        // Top or bottom row
            !(tile.first == gridX || tile.first == gridX + 3);           // Exclude corners

        bool isVerticalEdge = (tile.first == gridX || tile.first == gridX + 3) &&            // Left or right column
            !(tile.second == gridY || tile.second == gridY + 3);           // Exclude corners

        // Apply styles based on tile type
        if (isTopLeftCorner) {
            // Top-left corner
            br.fill_color[0] = 0.7f; // Red-ish color
            br.fill_color[1] = 0.1f;
            br.fill_color[2] = 0.1f;
        }
        else if (isTopRightCorner) {
            // Top-right corner
            br.fill_color[0] = 0.1f; // Green-ish color
            br.fill_color[1] = 0.7f;
            br.fill_color[2] = 0.1f;
        }
        else if (isBottomLeftCorner) {
            // Bottom-left corner
            br.fill_color[0] = 0.1f; // Blue-ish color
            br.fill_color[1] = 0.1f;
            br.fill_color[2] = 0.7f;
        }
        else if (isBottomRightCorner) {
            // Bottom-right corner
            br.fill_color[0] = 0.7f; // Yellow-ish color
            br.fill_color[1] = 0.7f;
            br.fill_color[2] = 0.1f;
        }
        else if (isHorizontalEdge) {
            // Horizontal edges
            br.fill_color[0] = 0.1f; // Example placeholder color
            br.fill_color[1] = 0.7f;
            br.fill_color[2] = 0.1f;
        }
        else if (isVerticalEdge) {
            // Vertical edges
            br.fill_color[0] = 0.1f; // Example placeholder color
            br.fill_color[1] = 0.1f;
            br.fill_color[2] = 0.7f;
        }
        else {
            // Inner tiles or other sections (if applicable)
            br.fill_color[0] = 0.5f; // Example placeholder color
            br.fill_color[1] = 0.5f;
            br.fill_color[2] = 0.5f;
        }

        br.outline_opacity = 0.5f; // Semi-transparent outline

        // Draw the tile
        graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
    }
}

void DepositZone::drawCircle() {
    graphics::Brush br;

    for (const auto& tile : tiles) {
        float xPos = tile.first * CELL_SIZE + CELL_SIZE / 2.0f;
        float yPos = tile.second * CELL_SIZE + CELL_SIZE / 2.0f;

        // Determine the position of the tile relative to the 4x4 circle
        bool isTopLeftCorner = (tile.first == gridX && tile.second == gridY);
        bool isTopRightCorner = (tile.first == gridX + 3 && tile.second == gridY);
        bool isBottomLeftCorner = (tile.first == gridX && tile.second == gridY + 3);
        bool isBottomRightCorner = (tile.first == gridX + 3 && tile.second == gridY + 3);

        bool isTopSideCorner = ((tile.first == gridX + 1 || tile.first == gridX + 2)
            && tile.second == gridY);
        bool isBottomSideCorner = ((tile.first == gridX + 1 || tile.first == gridX + 2)
            && tile.second == gridY + 3);
        bool isLeftSideCorner = (tile.first == gridX
            && (tile.second == gridY + 1 || tile.second == gridY + 2));
        bool isRightSideCorner = (tile.first == gridX + 3
            && (tile.second == gridY + 1 || tile.second == gridY + 2));

        bool isTopLeftCenter = (tile.first == gridX + 1 && tile.second == gridY + 1);
        bool isTopRightCenter = (tile.first == gridX + 2 && tile.second == gridY + 1);
        bool isBottomLeftCenter = (tile.first == gridX + 1 && tile.second == gridY + 2);
        bool isBottomRightCenter = (tile.first == gridX + 2 && tile.second == gridY + 2);

        // Apply styles based on tile type (unique color for each tile!)

        // 1) Four corners (each individually colored)
        if (isTopLeftCorner) {
            // (gridX, gridY)
            br.fill_color[0] = 1.0f;  // R
            br.fill_color[1] = 0.0f;  // G
            br.fill_color[2] = 0.0f;  // B  => Bright red
        }
        else if (isTopRightCorner) {
            // (gridX+3, gridY)
            br.fill_color[0] = 0.0f;
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;  // Bright green
        }
        else if (isBottomLeftCorner) {
            // (gridX, gridY+3)
            br.fill_color[0] = 0.0f;
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 1.0f;  // Bright blue
        }
        else if (isBottomRightCorner) {
            // (gridX+3, gridY+3)
            br.fill_color[0] = 1.0f;
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;  // Bright yellow
        }

        // 2) Edges (originally lumps 2 tiles each, so nest sub-if to differentiate)
        else if (isTopSideCorner) {
            // These are (gridX+1, gridY) and (gridX+2, gridY)
            if (tile.first == gridX + 1) {
                br.fill_color[0] = 1.0f; // Pink
                br.fill_color[1] = 0.5f;
                br.fill_color[2] = 0.7f;
            }
            else {
                br.fill_color[0] = 0.5f; // Light green
                br.fill_color[1] = 1.0f;
                br.fill_color[2] = 0.5f;
            }
        }
        else if (isBottomSideCorner) {
            // These are (gridX+1, gridY+3) and (gridX+2, gridY+3)
            if (tile.first == gridX + 1) {
                br.fill_color[0] = 0.5f;
                br.fill_color[1] = 0.5f;
                br.fill_color[2] = 1.0f; // Light blue
            }
            else {
                br.fill_color[0] = 0.3f;
                br.fill_color[1] = 0.7f;
                br.fill_color[2] = 1.0f; // Variation of light blue
            }
        }
        else if (isLeftSideCorner) {
            // These are (gridX, gridY+1) and (gridX, gridY+2)
            if (tile.second == gridY + 1) {
                br.fill_color[0] = 0.8f;
                br.fill_color[1] = 0.4f;
                br.fill_color[2] = 0.4f; // Salmon
            }
            else {
                br.fill_color[0] = 1.0f;
                br.fill_color[1] = 0.7f;
                br.fill_color[2] = 0.5f; // Light orange
            }
        }
        else if (isRightSideCorner) {
            // These are (gridX+3, gridY+1) and (gridX+3, gridY+2)
            if (tile.second == gridY + 1) {
                br.fill_color[0] = 0.5f;
                br.fill_color[1] = 1.0f;
                br.fill_color[2] = 0.7f; // Mint/pastel green
            }
            else {
                br.fill_color[0] = 0.5f;
                br.fill_color[1] = 1.0f;
                br.fill_color[2] = 1.0f; // Pastel cyan
            }
        }

        // 3) Four center tiles (each individually colored)
        else if (isTopLeftCenter) {
            br.fill_color[0] = 0.6f; // Purple
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 0.6f;
        }
        else if (isTopRightCenter) {
            br.fill_color[0] = 0.0f; // Darker purple
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 0.6f;
        }
        else if (isBottomLeftCenter) {
            br.fill_color[0] = 0.6f; // Dark red
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 0.0f;
        }
        else if (isBottomRightCenter) {
            br.fill_color[0] = 0.0f; // Dark green
            br.fill_color[1] = 0.6f;
            br.fill_color[2] = 0.0f;
        }

        // 4) Fallback (anything outside the 4×4, or not used)
        else {
            br.fill_color[0] = 0.1f; // Gray
            br.fill_color[1] = 0.5f;
            br.fill_color[2] = 0.1f;
        }

        // Semi-transparent outline
        br.outline_opacity = 0.5f;

        // Draw the tile
        graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
    }
}

