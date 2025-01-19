#include "depositzone.h"
#include "graphics.h"
#include <iostream>
#include "config.h"

DepositZone::DepositZone(GameState* state, int x, int y, Shape shape, bool horizontal)
    : GameObject(state, "DepositZone"), gridX(x + 1), gridY(y + UI_ROWS_ABOVE), shape(shape), horizontal(horizontal) {
    duration = 145.0f;  
    timer = duration;
    init(); 
    initializeVariants(); 
}

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
    initializeVariants(); 
}

void DepositZone::initializeVariants() {
    tileVariants.resize(tiles.size(), 0); 

    for (size_t i = 0; i < tiles.size(); ++i) {
        if (horizontal && i > 0 && i < tiles.size() - 1) {
            tileVariants[i] = 1 + (rand() % 4); 
        }
        else {
            tileVariants[i] = 0;
        }
    }
}

// Create a straight line (horizontal or vertical)
void DepositZone::createStraightLine() {
    if (horizontal) { // Horizontal line
        for (int x = 1; x < 13; ++x) { 
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
    int adjustedGridX = std::min(gridX, 1 + PLAYABLE_COLUMNS - 4); 
    int adjustedGridY = std::min(gridY, UI_ROWS_ABOVE + PLAYABLE_ROWS - 4); 

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
            if (!((x == adjustedGridX && y == adjustedGridY) ||                 
                (x == adjustedGridX + 3 && y == adjustedGridY) ||             
                (x == adjustedGridX && y == adjustedGridY + 3) ||
                (x == adjustedGridX + 3 && y == adjustedGridY + 3))) {
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

void DepositZone::update(float dt) {
    if (dt > 0.1f) dt = 0.1f; 

    if (timer > 0) {
        timer -= dt; 
    }
}

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
            if (i == 0) {
                br.texture = ASSET_PATH + "zones/HORIZ-LEFT.png";
            }
            else if (i == tiles.size() - 1) {
                br.texture = ASSET_PATH + "zones/HORIZ-RIGHT.png";
            }
            else {
                int variant = tileVariants[i];
                br.texture = ASSET_PATH + "zones/HORIZ-MID" + std::to_string(variant) + ".png";
            }
        }
        else {
            if (i == 0) {
                br.texture = ASSET_PATH + "zones/VERT-TOP.png";
            }
            else if (i == tiles.size() - 1) {
                br.texture = ASSET_PATH + "zones/VERT-BOT.png";
            }
            else {
                br.texture = ASSET_PATH + "zones/VERT-MID.png";
            }
        }

        br.outline_opacity = 0.0f; 

        graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
    }
}

void DepositZone::drawDonut() {
    graphics::Brush br;
    for (const auto& tile : tiles) {
        float xPos = tile.first * CELL_SIZE + CELL_SIZE / 2.0f;
        float yPos = tile.second * CELL_SIZE + CELL_SIZE / 2.0f;

        int effectiveGridY = (gridY + 3 >= PLAYABLE_ROWS + UI_ROWS_ABOVE) ?
            PLAYABLE_ROWS + UI_ROWS_ABOVE - 4 : gridY;

        bool isTopLeftCorner = (tile.first == gridX && tile.second == effectiveGridY);
        bool isTopRightCorner = (tile.first == gridX + 3 && tile.second == effectiveGridY);
        bool isBottomLeftCorner = (tile.first == gridX && tile.second == effectiveGridY + 3);
        bool isBottomRightCorner = (tile.first == gridX + 3 && tile.second == effectiveGridY + 3);

        bool isHorizontalEdge =
            ((tile.second == effectiveGridY) || (tile.second == effectiveGridY + 3))    
            && !(tile.first == gridX || tile.first == gridX + 3);

        bool isVerticalEdge =
            ((tile.first == gridX) || (tile.first == gridX + 3))      
            && !(tile.second == effectiveGridY || tile.second == effectiveGridY + 3);   

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

        if (tx < 0 || tx >= GRID_WIDTH || ty < 0 || ty >= GRID_HEIGHT) {
            continue;
        }

        float xPos = tx * CELL_SIZE + CELL_SIZE / 2.0f;
        float yPos = ty * CELL_SIZE + CELL_SIZE / 2.0f;

        bool isTopSideCorner = ((tx == gridX + 1 || tx == gridX + 2) && ty == gridY);
        bool isBottomSideCorner = ((tx == gridX + 1 || tx == gridX + 2) && ty == gridY + 3);
        bool isLeftSideCorner = (tx == gridX && (ty == gridY + 1 || ty == gridY + 2));
        bool isRightSideCorner = (tx == gridX + 3 && (ty == gridY + 1 || ty == gridY + 2));

        bool isTopLeftCenter = (tx == gridX + 1 && ty == gridY + 1);
        bool isTopRightCenter = (tx == gridX + 2 && ty == gridY + 1);
        bool isBottomLeftCenter = (tx == gridX + 1 && ty == gridY + 2);
        bool isBottomRightCenter = (tx == gridX + 2 && ty == gridY + 2);

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

        graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
    }
}
