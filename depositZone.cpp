#include "DepositZone.h"
#include "graphics.h"
#include <iostream>
#include "config.h"

// Constructor
DepositZone::DepositZone(GameState* state, int x, int y, Shape shape, bool horizontal)
    : GameObject(state, "DepositZone"), gridX(x + 1), gridY(y + UI_ROWS_ABOVE), shape(shape), horizontal(horizontal) {
    duration = 145.0f;  // 10 seconds active time
    timer = duration;
    init(); // Calculate zone tiles
    initializeVariants(); // Precompute random variants
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
    initializeVariants(); // Ensure variants are updated after tiles are created
}

// Initialize random variants for middle tiles
void DepositZone::initializeVariants() {
    tileVariants.resize(tiles.size(), 0); // Match the size of tiles

    for (size_t i = 0; i < tiles.size(); ++i) {
        if (horizontal && i > 0 && i < tiles.size() - 1) {
            // Generate random variants for horizontal middle tiles
            tileVariants[i] = 1 + (rand() % 4); // Random number between 1 and 4
        }
        else {
            // No variant needed for edge or vertical tiles
            tileVariants[i] = 0;
        }
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
                br.texture = ASSET_PATH + "zones/HORIZ-LEFT.png";
            }
            else if (i == tiles.size() - 1) {
                // Right edge
                br.texture = ASSET_PATH + "zones/HORIZ-RIGHT.png";
            }
            else {
                // Use precomputed random variant for middle tiles
                int variant = tileVariants[i];
                br.texture = ASSET_PATH + "zones/HORIZ-MID" + std::to_string(variant) + ".png";
            }
        }
        else {
            // Vertical straight line
            if (i == 0) {
                // Top edge
                br.texture = ASSET_PATH + "zones/VERT-TOP.png";
            }
            else if (i == tiles.size() - 1) {
                // Bottom edge
                br.texture = ASSET_PATH + "zones/VERT-BOT.png";
            }
            else {
                // Middle tiles
                br.texture = ASSET_PATH + "zones/VERT-MID.png";
            }
        }

        br.outline_opacity = 0.0f; // Remove outlines for textures

        // Draw the tile
        graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
    }
}

void DepositZone::drawDonut() {
    graphics::Brush br;
    for (const auto& tile : tiles) {
        float xPos = tile.first * CELL_SIZE + CELL_SIZE / 2.0f;
        float yPos = tile.second * CELL_SIZE + CELL_SIZE / 2.0f;

        // First, validate if this tile position is valid for the intended shape
        // If gridY + 3 would exceed playable area, adjust gridY to keep donut in bounds
        int effectiveGridY = (gridY + 3 >= PLAYABLE_ROWS + UI_ROWS_ABOVE) ?
            PLAYABLE_ROWS + UI_ROWS_ABOVE - 4 : gridY;

        // Now use effectiveGridY instead of gridY for all position checks
        bool isTopLeftCorner = (tile.first == gridX && tile.second == effectiveGridY);
        bool isTopRightCorner = (tile.first == gridX + 3 && tile.second == effectiveGridY);
        bool isBottomLeftCorner = (tile.first == gridX && tile.second == effectiveGridY + 3);
        bool isBottomRightCorner = (tile.first == gridX + 3 && tile.second == effectiveGridY + 3);

        bool isHorizontalEdge =
            ((tile.second == effectiveGridY) || (tile.second == effectiveGridY + 3))    // top or bottom row
            && !(tile.first == gridX || tile.first == gridX + 3);     // exclude corners

        bool isVerticalEdge =
            ((tile.first == gridX) || (tile.first == gridX + 3))      // left or right column
            && !(tile.second == effectiveGridY || tile.second == effectiveGridY + 3);   // exclude corners

        // Rest of the function remains the same...
        if (isTopLeftCorner) {
            br.texture = ASSET_PATH + "zones/DONTL.png";
        }
        else if (isTopRightCorner) {
            br.texture = ASSET_PATH + "zones/DONTR.png";
        }
        else if (isBottomLeftCorner) {
            br.texture = ASSET_PATH + "zones/DONBL.png";
        }
        else if (isBottomRightCorner) {
            br.texture = ASSET_PATH + "zones/DONBR.png";
        }
        else if (isHorizontalEdge) {
            br.texture = ASSET_PATH + "zones/HORIZ-MID4.png";
        }
        else if (isVerticalEdge) {
            br.texture = ASSET_PATH + "zones/VERT-MID.png";
        }

        br.outline_opacity = 0.0f;
        graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
    }
}

void DepositZone::drawCircle() {
    graphics::Brush br;

    for (const auto& tile : tiles) {
        int tx = tile.first;
        int ty = tile.second;

        // 1) Skip tiles that fall outside the valid grid range
        if (tx < 0 || tx >= GRID_WIDTH || ty < 0 || ty >= GRID_HEIGHT) {
            continue;
        }

        // 2) Compute center for drawing
        float xPos = tx * CELL_SIZE + CELL_SIZE / 2.0f;
        float yPos = ty * CELL_SIZE + CELL_SIZE / 2.0f;

        // 3) The same logic you already use to figure out which texture to apply
        bool isTopSideCorner = ((tx == gridX + 1 || tx == gridX + 2) && ty == gridY);
        bool isBottomSideCorner = ((tx == gridX + 1 || tx == gridX + 2) && ty == gridY + 3);
        bool isLeftSideCorner = (tx == gridX && (ty == gridY + 1 || ty == gridY + 2));
        bool isRightSideCorner = (tx == gridX + 3 && (ty == gridY + 1 || ty == gridY + 2));

        bool isTopLeftCenter = (tx == gridX + 1 && ty == gridY + 1);
        bool isTopRightCenter = (tx == gridX + 2 && ty == gridY + 1);
        bool isBottomLeftCenter = (tx == gridX + 1 && ty == gridY + 2);
        bool isBottomRightCenter = (tx == gridX + 2 && ty == gridY + 2);

        // TEXTURE ASSIGNMENTS
        if (isTopSideCorner) {
            if (tx == gridX + 1)
                br.texture = ASSET_PATH + "zones/TOPLEFT1.png";
            else
                br.texture = ASSET_PATH + "zones/TOPRIGHT1.png";
        }
        else if (isBottomSideCorner) {
            if (tx == gridX + 1)
                br.texture = ASSET_PATH + "zones/BOTLEFT2.png";
            else
                br.texture = ASSET_PATH + "zones/BOTRIGHT2.png";
        }
        else if (isLeftSideCorner) {
            if (ty == gridY + 1)
                br.texture = ASSET_PATH + "zones/TOPLEFT2.png";
            else
                br.texture = ASSET_PATH + "zones/BOTLEFT1.png";
        }
        else if (isRightSideCorner) {
            if (ty == gridY + 1)
                br.texture = ASSET_PATH + "zones/TOPRIGHT1.png";
            else
                br.texture = ASSET_PATH + "zones/BOTRIGHT1.png";
        }
        else if (isTopLeftCenter) {
            br.texture = ASSET_PATH + "zones/CENTER-TOPLEFT.png";
        }
        else if (isTopRightCenter) {
            br.texture = ASSET_PATH + "zones/CENTER-TOPRIGHT.png";
        }
        else if (isBottomLeftCenter) {
            br.texture = ASSET_PATH + "zones/CENTER-BOTLEFT.png";
        }
        else if (isBottomRightCenter) {
            br.texture = ASSET_PATH + "zones/CENTER-BOTRIGHT.png";
        }
        else {
            continue;
        }

        br.outline_opacity = 0.0f;

        // 4) Draw the tile
        graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
    }
}
