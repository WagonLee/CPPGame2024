#include "GridRenderer.h"
#include "graphics.h"
#include "gameState.h"

// Define the grid state
std::vector<std::vector<Tile>> gridState(GRID_HEIGHT, std::vector<Tile>(GRID_WIDTH));

// Initialize the grid
void initGrid() {
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            gridState[row][col] = Tile(1.0f, 1.0f, 1.0f); // Default to white for testing
            // --- Handle Specific Tile Types ---

            // 1. Top-most row (individually addressable tiles)
            if (row == 0) {
                if (col == 0) {
                    gridState[row][col] = Tile(1.0f, 0.0f, 0.0f); // Top-left corner (Red)
                }
                else if (col == 1) {
                    gridState[row][col] = Tile(0.9f, 0.1f, 0.1f); // Tile at (0, 1)
                }
                else if (col == 2) {
                    gridState[row][col] = Tile(0.8f, 0.2f, 0.2f); // Tile at (0, 2)
                }
                else if (col == 3) {
                    gridState[row][col] = Tile(0.7f, 0.3f, 0.3f); // Tile at (0, 3)
                }
                else if (col == 4) {
                    gridState[row][col] = Tile(0.6f, 0.4f, 0.4f); // Tile at (0, 4)
                }
                else if (col == 5) {
                    gridState[row][col] = Tile(0.5f, 0.5f, 0.5f); // Tile at (0, 5)
                }
                else if (col == 6) {
                    gridState[row][col] = Tile(0.4f, 0.6f, 0.6f); // Tile at (0, 6)
                }
                else if (col == 7) {
                    gridState[row][col] = Tile(0.3f, 0.7f, 0.7f); // Tile at (0, 7)
                }
                else if (col == 8) {
                    gridState[row][col] = Tile(0.2f, 0.8f, 0.8f); // Tile at (0, 8)
                }
                else if (col == 9) {
                    gridState[row][col] = Tile(0.1f, 0.9f, 0.9f); // Tile at (0, 9)
                }
                else if (col == 10) {
                    gridState[row][col] = Tile(0.2f, 0.9f, 0.1f); // Tile at (0, 10)
                }
                else if (col == 11) {
                    gridState[row][col] = Tile(0.3f, 0.2f, 0.9f); // Tile at (0, 11)
                }
                else if (col == GRID_WIDTH - 2) {
                    gridState[row][col] = Tile(0.2f, 0.2f, 0.7f); // Second to last tile
                }
                else if (col == GRID_WIDTH - 1) {
                    gridState[row][col] = Tile(0.0f, 1.0f, 0.0f); // Top-right corner (Green)
                }
            }

            // 2. Corners of the entire window (excluding top row tiles)
            else if (row == GRID_HEIGHT - 1 && col == 0) {
                gridState[row][col] = Tile(1.0f, 0.5f, 0.0f); // Bottom-left corner (Orange)
            }
            else if (row == GRID_HEIGHT - 1 && col == GRID_WIDTH - 1) {
                gridState[row][col] = Tile(1.0f, 1.0f, 0.0f); // Bottom-right corner (Yellow)
            }

            // 3. Bottom corners of the grid border
            else if (row == UI_ROWS_ABOVE + PLAYABLE_ROWS && col == 0) {
                gridState[row][col] = Tile(0.0f, 1.0f, 1.0f); // Bottom-left grid border (Cyan)
            }
            else if (row == UI_ROWS_ABOVE + PLAYABLE_ROWS && col == PLAYABLE_COLUMNS + 1) {
                gridState[row][col] = Tile(1.0f, 0.0f, 1.0f); // Bottom-right grid border (Pink)
            }

            // 4. Left and right border columns (Blue)
            else if (col == 0 || col == GRID_WIDTH - 1) {
                gridState[row][col] = Tile(0.2f, 0.0f, 0.5f); // Blue
            }

            // 5. Rows above and below the playable grid (Blue)
            else if (row < UI_ROWS_ABOVE || row >= UI_ROWS_ABOVE + PLAYABLE_ROWS) {
                gridState[row][col] = Tile(0.0f, 0.5f, 1.0f); // Light Blue
            }

            // 6. Playable rows (Alternating Black and White)
            else {
                if ((row + col) % 2 == 0) {
                    gridState[row][col] = Tile(0.0f, 0.0f, 0.0f); // Black
                }
                else {
                    gridState[row][col] = Tile(1.0f, 1.0f, 1.0f); // White
                }
            }
        }
    }
}

// Update the grid dynamically
void updateGrid() {
    Player* player = nullptr;

    // Locate the player object
    for (const auto& obj : GameState::getInstance()->getGameObjects()) {
        player = dynamic_cast<Player*>(obj.get());
        if (player) {
            break; // Found the player
        }
    }

    // Ensure the player exists and is dead
    if (player && !player->getIsAlive()) {
        // Set each tile in the top row to red
        gridState[0][1] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][2] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][3] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][4] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][5] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][6] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][7] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][8] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][9] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][10] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][11] = Tile(1.0f, 0.0f, 0.0f);
        gridState[0][12] = Tile(1.0f, 0.0f, 0.0f);

    }
}


// Render the grid
void drawGrid() {
    graphics::Brush br;

    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            const auto& tile = gridState[row][col];
            br.fill_color[0] = tile.r;
            br.fill_color[1] = tile.g;
            br.fill_color[2] = tile.b;

            // Use texture if available
            if (!tile.texture.empty()) {
                br.texture = tile.texture;
            }
            else {
                br.texture = "";
            }

            br.fill_opacity = 1.0f;
            br.outline_opacity = 0.0f;

            float x = col * CELL_SIZE + CELL_SIZE / 2;
            float y = row * CELL_SIZE + CELL_SIZE / 2;
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, br);
        }
    }
}
