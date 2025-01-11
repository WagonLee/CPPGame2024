#include "GridRenderer.h"
#include "graphics.h"
#include "gameState.h"

// Define the grid state
std::vector<std::vector<Tile>> gridState(GRID_HEIGHT, std::vector<Tile>(GRID_WIDTH));

// Initialize the grid
void initGrid() {
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {

            // Default all tiles to white initially
            gridState[row][col] = Tile(1.0f, 1.0f, 1.0f);

            // 1. Top-most row
            if (row == 0) {
                if (col == 0) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/TL-CORNER.png"); // Top-left corner (Red)
                }
                else if (col == 1) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 2) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 3) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 4) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 5) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 6) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 7) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 8) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 9) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 10) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 11) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == 12) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                }
                else if (col == GRID_WIDTH - 1) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/TR-CORNER.png"); // Top-right corner (Red)
                }
            }

            // 2. Bottom corners of the entire window
            else if (row == GRID_HEIGHT - 1 && col == 0) {
                gridState[row][col] = Tile(ASSET_PATH + "grid/TR-CORNER.png"); // Bottom-left corner (Orange)
            }
            else if (row == GRID_HEIGHT - 1 && col == GRID_WIDTH - 1) {
                gridState[row][col] = Tile(1.0f, 1.0f, 0.0f); // Bottom-right corner (Yellow)
            }

            // 3. Entire last row (if not corners)
            else if (row == GRID_HEIGHT - 1) {
                gridState[row][col] = Tile(0.2f, 0.0f, 0.5f); // Blue
            }

            // 4. Additional rows well below the playable grid
            else if (row >= UI_ROWS_ABOVE + PLAYABLE_ROWS + 1) {
                int relativeRow = row - (UI_ROWS_ABOVE + PLAYABLE_ROWS + 1);

                switch (relativeRow) {
                case 0:
                    if (col == 0) {
                        gridState[row][col] = Tile(0.7f, 0.7f, 0.2f);
                    }
                    else if (col == 1) {
                        gridState[row][col] = Tile(0.5f, 0.0f, 0.9f);
                    }
                    else if (col == 2) {
                        gridState[row][col] = Tile(0.0f, 0.5f, 0.9f);
                    }
                    else if (col == 3) {
                        gridState[row][col] = Tile(0.6f, 0.3f, 0.8f);
                    }
                    else if (col == 4) {
                        gridState[row][col] = Tile(0.4f, 0.8f, 0.1f);
                    }
                    else if (col == 5) {
                        gridState[row][col] = Tile(0.3f, 0.2f, 0.6f);
                    }
                    else if (col == 6) {
                        gridState[row][col] = Tile(0.2f, 0.9f, 0.2f);
                    }
                    else if (col == 7) {
                        gridState[row][col] = Tile(0.5f, 0.1f, 0.9f);
                    }
                    else if (col == 8) {
                        gridState[row][col] = Tile(0.1f, 0.5f, 0.3f);
                    }
                    else if (col == 9) {
                        gridState[row][col] = Tile(0.9f, 0.3f, 0.2f);
                    }
                    else if (col == 10) {
                        gridState[row][col] = Tile(0.7f, 0.6f, 0.8f);
                    }
                    else if (col == 11) {
                        gridState[row][col] = Tile(0.1f, 0.1f, 0.1f);
                    }
                    else if (col == 12) {
                        gridState[row][col] = Tile(0.1f, 0.9f, 0.1f);
                    }
                    else if (col == GRID_WIDTH - 1) {
                        gridState[row][col] = Tile(0.0f, 1.0f, 0.0f); // Bruh
                    }
                    break;

                case 1:
                    if (col == 0) {
                        gridState[row][col] = Tile(0.3f, 0.2f, 0.1f);
                    }
                    else if (col == 1) {
                        gridState[row][col] = Tile(0.8f, 0.5f, 0.7f);
                    }
                    else if (col == 2) {
                        gridState[row][col] = Tile(0.2f, 0.4f, 0.6f);
                    }
                    else if (col == 3) {
                        gridState[row][col] = Tile(0.5f, 0.3f, 0.2f);
                    }
                    else if (col == 4) {
                        gridState[row][col] = Tile(0.1f, 0.7f, 0.5f);
                    }
                    else if (col == 5) {
                        gridState[row][col] = Tile(0.9f, 0.2f, 0.8f);
                    }
                    else if (col == 6) {
                        gridState[row][col] = Tile(0.3f, 0.3f, 0.3f);
                    }
                    else if (col == 7) {
                        gridState[row][col] = Tile(0.8f, 0.6f, 0.2f);
                    }
                    else if (col == 8) {
                        gridState[row][col] = Tile(0.8f, 0.9f, 0.1f);
                    }
                    else if (col == 9) {
                        gridState[row][col] = Tile(0.9f, 0.3f, 0.2f);
                    }
                    else if (col == 10) {
                        gridState[row][col] = Tile(0.1f, 0.6f, 0.8f);
                    }
                    else if (col == 11) {
                        gridState[row][col] = Tile(0.1f, 0.5f, 0.1f);
                    }
                    else if (col == 12) {
                        gridState[row][col] = Tile(0.1f, 0.9f, 0.3f);
                    }
                    else if (col == GRID_WIDTH - 1) {
                        gridState[row][col] = Tile(0.0f, 1.0f, 0.0f); // Bruh
                    }
                    break;

                case 2:
                    if (col == 0) {
                        gridState[row][col] = Tile(0.4f, 0.4f, 0.9f);
                    }
                    else if (col == 1) {
                        gridState[row][col] = Tile(0.3f, 0.6f, 0.7f);
                    }
                    else if (col == 2) {
                        gridState[row][col] = Tile(0.7f, 0.2f, 0.1f);
                    }
                    else if (col == 3) {
                        gridState[row][col] = Tile(0.5f, 0.8f, 0.2f);
                    }
                    else if (col == 4) {
                        gridState[row][col] = Tile(0.6f, 0.3f, 0.4f);
                    }
                    else if (col == 5) {
                        gridState[row][col] = Tile(0.9f, 0.9f, 0.5f);
                    }
                    else if (col == 6) {
                        gridState[row][col] = Tile(0.2f, 0.6f, 0.9f);
                    }
                    else if (col == 7) {
                        gridState[row][col] = Tile(0.4f, 0.9f, 0.7f);
                    }
                    else if (col == 8) {
                        gridState[row][col] = Tile(0.1f, 0.7f, 0.3f);
                    }
                    else if (col == 9) {
                        gridState[row][col] = Tile(0.9f, 0.3f, 0.2f);
                    }
                    else if (col == 10) {
                        gridState[row][col] = Tile(0.7f, 0.6f, 0.1f);
                    }
                    else if (col == 11) {
                        gridState[row][col] = Tile(0.1f, 0.1f, 0.5f);
                    }
                    else if (col == 12) {
                        gridState[row][col] = Tile(0.3f, 0.9f, 0.1f);
                    }
                    else if (col == GRID_WIDTH - 1) {
                        gridState[row][col] = Tile(0.0f, 1.0f, 0.0f); // Bruh
                    }
                    break;
                case 3:
                    if (col == 0) {
                        gridState[row][col] = Tile(0.7f, 0.7f, 0.2f);
                    }
                    else if (col == 1) {
                        gridState[row][col] = Tile(0.5f, 0.0f, 0.9f);
                    }
                    else if (col == 2) {
                        gridState[row][col] = Tile(0.0f, 0.5f, 0.9f);
                    }
                    else if (col == 3) {
                        gridState[row][col] = Tile(0.6f, 0.3f, 0.8f);
                    }
                    else if (col == 4) {
                        gridState[row][col] = Tile(0.4f, 0.8f, 0.1f);
                    }
                    else if (col == 5) {
                        gridState[row][col] = Tile(0.3f, 0.2f, 0.6f);
                    }
                    else if (col == 6) {
                        gridState[row][col] = Tile(0.2f, 0.9f, 0.2f);
                    }
                    else if (col == 7) {
                        gridState[row][col] = Tile(0.5f, 0.1f, 0.9f);
                    }
                    else if (col == 8) {
                        gridState[row][col] = Tile(0.1f, 0.5f, 0.3f);
                    }
                    else if (col == 9) {
                        gridState[row][col] = Tile(0.9f, 0.3f, 0.2f);
                    }
                    else if (col == 10) {
                        gridState[row][col] = Tile(0.7f, 0.6f, 0.8f);
                    }
                    else if (col == 11) {
                        gridState[row][col] = Tile(0.1f, 0.1f, 0.1f);
                    }
                    else if (col == 12) {
                        gridState[row][col] = Tile(0.1f, 0.9f, 0.1f);
                    }
                    else if (col == GRID_WIDTH - 1) {
                        gridState[row][col] = Tile(0.0f, 1.0f, 0.0f); // Bruh
                    }
                    break;
                }
            }

            // 5. Bottom corners of the grid border
            else if (row == UI_ROWS_ABOVE + PLAYABLE_ROWS && col == 0) {
                gridState[row][col] = Tile(0.0f, 1.0f, 1.0f); // Bottom-left grid border (Cyan)
            }
            else if (row == UI_ROWS_ABOVE + PLAYABLE_ROWS && col == PLAYABLE_COLUMNS + 1) {
                gridState[row][col] = Tile(1.0f, 0.0f, 1.0f); // Bottom-right grid border (Pink)
            }

            // 6. Left and right border columns
            else if (col == 0 || col == GRID_WIDTH - 1) {
                gridState[row][col] = Tile(0.2f, 0.0f, 0.5f); // Blue
            }

            // 7. Rows above and below the playable grid
            else if (row < UI_ROWS_ABOVE || row >= UI_ROWS_ABOVE + PLAYABLE_ROWS) {
                gridState[row][col] = Tile(0.0f, 0.5f, 1.0f); // Light Blue
            }

            // 8. Playable rows (alternating black/white)
            else {
                if ((row + col) % 2 == 0) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/GRID.png");
                }
                else {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/GRID.png");
                }
            }
        }
    }
}


// Update the grid dynamically
void updateGrid() {
    Player* player = nullptr;
    auto* gameState = GameState::getInstance();

    // EFFECT 1

    // Check if an enemy was recently killed
    // Check if an enemy was recently killed
    if (gameState->enemyKilled) {
        // Turn the top row blue
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

        // Reset the top row if the current time has passed the end time
        if (graphics::getGlobalTime() >= gameState->enemyKillEndTime) {
            gameState->enemyKilled = false;

            // Reset the top row to its init colors
            gridState[0][1] = Tile(0.9f, 0.1f, 0.1f); // Tile at (0, 1)
            gridState[0][2] = Tile(0.8f, 0.2f, 0.2f); // Tile at (0, 2)
            gridState[0][3] = Tile(0.7f, 0.3f, 0.3f); // Tile at (0, 3)
            gridState[0][4] = Tile(0.6f, 0.4f, 0.4f); // Tile at (0, 4)
            gridState[0][5] = Tile(0.5f, 0.5f, 0.5f); // Tile at (0, 5)
            gridState[0][6] = Tile(0.4f, 0.6f, 0.6f); // Tile at (0, 6)
            gridState[0][7] = Tile(0.3f, 0.7f, 0.7f); // Tile at (0, 7)
            gridState[0][8] = Tile(0.2f, 0.8f, 0.8f); // Tile at (0, 8)
            gridState[0][9] = Tile(0.1f, 0.9f, 0.9f); // Tile at (0, 9)
            gridState[0][10] = Tile(0.2f, 0.9f, 0.1f); // Tile at (0, 10)
            gridState[0][11] = Tile(0.3f, 0.2f, 0.9f); // Tile at (0, 11)
            gridState[0][12] = Tile(1.0f, 0.2f, 1.0f); // Tile at (0, 12)
        }
    }

    // EFFECT 2

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

            br.outline_opacity = 0.0f;

            float x = col * CELL_SIZE + CELL_SIZE / 2;
            float y = row * CELL_SIZE + CELL_SIZE / 2;
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, br);
        }
    }
}
