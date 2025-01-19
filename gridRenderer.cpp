#include "gridrenderer.h"
#include "graphics.h"
#include "gamestate.h"

// Define the grid state
std::vector<std::vector<Tile>> gridState(GRID_HEIGHT, std::vector<Tile>(GRID_WIDTH));

// Initialize the grid - NOT VERY PROUD OF THIS... I FOUND MORE ELEGANT SOLUTIONS LATER, BUT THIS IS VERY MODULAR AND I NEEDED THAT AT THE TIME. 
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
                    gridState[row][col] = Tile(ASSET_PATH + "grid/BR-CORNER.png");
                }
                else if (col == 2) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/B.png");
                }
                else if (col == 3) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/Y.png");
                }
                else if (col == 4) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/T.png");
                }
                else if (col == 5) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/E.png");
                }
                else if (col == 6) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/R.png");
                }
                else if (col == 7) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/A.png");
                }
                else if (col == 8) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/I.png");
                }
                else if (col == 9) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/D.png");
                }
                else if (col == 10) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/E.png");
                }
                else if (col == 11) {
                    gridState[row][col] = Tile(ASSET_PATH + "chars/R.png");
                }
                else if (col == 12) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/BL-CORNER.png");
                }
                else if (col == GRID_WIDTH - 1) {
                    gridState[row][col] = Tile(ASSET_PATH + "grid/TR-CORNER.png"); // Top-right corner 
                }
            }

            // 2. Bottom corners of the entire window
            else if (row == GRID_HEIGHT - 1 && col == 0) {
                gridState[row][col] = Tile(ASSET_PATH + "grid/BL-CORNER.png"); // Bottom-left corner
            }
            else if (row == GRID_HEIGHT - 1 && col == GRID_WIDTH - 1) {
                gridState[row][col] = Tile(ASSET_PATH + "grid/BR-CORNER.png"); // Bottom-right corner 
            }

            // 3. Entire last row (if not corners)
            else if (row == GRID_HEIGHT - 1) {
                gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
            }

            // 4. Additional rows well below the playable grid
            else if (row >= UI_ROWS_ABOVE + PLAYABLE_ROWS + 1) {
                int relativeRow = row - (UI_ROWS_ABOVE + PLAYABLE_ROWS + 1);

                switch (relativeRow) {
                case 0:
                    if (col == 0) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/VERTICAL-EDGE.png");
                    }
                    else if (col == 1) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/S.png");
                    }
                    else if (col == 2) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/C.png");
                    }
                    else if (col == 3) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/R.png");
                    }
                    else if (col == 4) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/0.png");
                    }
                    else if (col == 5) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/0.png");
                    }
                    else if (col == 6) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/0.png");
                    }
                    else if (col == 7) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/0.png");
                    }
                    else if (col == 8) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/0.png");
                    }
                    else if (col == 9) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/0.png");
                    }
                    else if (col == 10) {
                        gridState[row][col] = Tile(ASSET_PATH    + "chars/X.png");
                    }
                    else if (col == 11) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/0.png");
                    }
                    else if (col == 12) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/1.png");
                    }
                    else if (col == GRID_WIDTH - 1) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/VERTICAL-EDGE.png");
                    }
                    break;

                case 1:
                {
                    if (col == 0) {
                        // Left corner
                        gridState[row][col] = Tile(ASSET_PATH + "grid/ML-CORNER.png");
                    }
                    else if (col == GRID_WIDTH - 1) {
                        // Right corner
                        gridState[row][col] = Tile(ASSET_PATH + "grid/MR-CORNER.png");
                    }
                    else {
                        // Everything in between
                        gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
                    }
                    break;
                }


                case 2:
                    if (col == 0) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/VERTICAL-EDGE.png");
                    }
                    else if (col == 1) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/P.png");
                    }
                    else if (col == 2) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/A.png");
                    }
                    else if (col == 3) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/Y.png");
                    }
                    else if (col == 4) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/L.png");
                    }
                    else if (col == 5) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/O.png");
                    }
                    else if (col == 6) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/A.png");
                    }
                    else if (col == 7) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/D.png");
                    }
                    else if (col == 8) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/S.png");
                    }
                    else if (col == 9) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/POWERUP1.png");
                    }
                    else if (col == 10) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/POWERUP2.png");
                    }
                    else if (col == 11) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/POWERUP3.png");
                    }
                    else if (col == 12) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/POWERUP4.png");
                    }
                    else if (col == GRID_WIDTH - 1) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/VERTICAL-EDGE.png");
                    }
                    break;
                case 3:
                    if (col == 0) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/VERTICAL-EDGE.png");
                    }
                    else if (col == 1) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/R.png");
                    }
                    else if (col == 2) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/A.png");
                    }
                    else if (col == 3) {
                        gridState[row][col] = Tile(ASSET_PATH + "chars/M.png");
                    }
                    else if (col == 4) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/TALLY-OFF1.png");
                    }
                    else if (col == 5) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/TALLY-OFF1.png");
                    }
                    else if (col == 6) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/TALLY-OFF1.png");
                    }
                    else if (col == 7) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/TALLY-OFF1.png");
                    }
                    else if (col == 8) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/TALLY-OFF1.png");
                    }
                    else if (col == 9) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/TALLY-OFF2.png");
                    }
                    else if (col == 10) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/TALLY-OFF2.png");
                    }
                    else if (col == 11) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/TALLY-OFF2.png");
                    }
                    else if (col == 12) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/TALLY-OFF2.png");
                    }
                    else if (col == GRID_WIDTH - 1) {
                        gridState[row][col] = Tile(ASSET_PATH + "grid/VERTICAL-EDGE.png");
                    }
                    break;
                }
            }

            // 5. Bottom corners of the grid border
            else if (row == UI_ROWS_ABOVE + PLAYABLE_ROWS && col == 0) {
                gridState[row][col] = Tile(ASSET_PATH + "grid/ML-CORNER.png");
            }
            else if (row == UI_ROWS_ABOVE + PLAYABLE_ROWS && col == PLAYABLE_COLUMNS + 1) {
                gridState[row][col] = Tile(ASSET_PATH + "grid/MR-CORNER.png");
            }

            // 6. Left and right border columns
            else if (col == 0 || col == GRID_WIDTH - 1) {
                gridState[row][col] = Tile(ASSET_PATH + "grid/VERTICAL-EDGE.png");
            }

            // 7. Rows above and below the playable grid
            else if (row < UI_ROWS_ABOVE || row >= UI_ROWS_ABOVE + PLAYABLE_ROWS) {
                gridState[row][col] = Tile(ASSET_PATH + "grid/HORIZONTAL-EDGE.png");
            }

            // 8. Playable rows 
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
    if (gameState->enemyKilled) {
        // Turn the top row blue
        gridState[0][2] = Tile(0.0f, 0.0f, 0.0f);
        gridState[0][3] = Tile(ASSET_PATH + "grid/PURGED.png");
        gridState[0][4] = Tile(ASSET_PATH + "chars/P.png");
        gridState[0][5] = Tile(ASSET_PATH + "chars/U.png");
        gridState[0][6] = Tile(ASSET_PATH + "chars/R.png");
        gridState[0][7] = Tile(ASSET_PATH + "chars/G.png");
        gridState[0][8] = Tile(ASSET_PATH + "chars/E.png");
        gridState[0][9] = Tile(ASSET_PATH + "chars/D.png");
        gridState[0][10] = Tile(ASSET_PATH + "grid/PURGED.png");
        gridState[0][11] = Tile(0.0f, 0.0f, 0.0f);

        // Reset the top row if the current time has passed the end time
        if (graphics::getGlobalTime() >= gameState->enemyKillEndTime) {
            gameState->enemyKilled = false;

            // Reset the top row to its init colors
            gridState[0][2] = Tile(ASSET_PATH + "chars/B.png");
            gridState[0][3] = Tile(ASSET_PATH + "chars/Y.png");
            gridState[0][4] = Tile(ASSET_PATH + "chars/T.png");
            gridState[0][5] = Tile(ASSET_PATH + "chars/E.png");
            gridState[0][6] = Tile(ASSET_PATH + "chars/R.png");
            gridState[0][7] = Tile(ASSET_PATH + "chars/A.png");
            gridState[0][8] = Tile(ASSET_PATH + "chars/I.png");
            gridState[0][9] = Tile(ASSET_PATH + "chars/D.png");
            gridState[0][10] = Tile(ASSET_PATH + "chars/E.png");
            gridState[0][11] = Tile(ASSET_PATH + "chars/R.png");
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
        gridState[0][2] = Tile(0.0f, 0.0f, 0.0f);
        gridState[0][3] = Tile(0.0f, 0.0f, 0.0f);
        gridState[0][4] = Tile(ASSET_PATH + "grid/PWND.png");
        gridState[0][5] = Tile(ASSET_PATH + "chars/P.png");
        gridState[0][6] = Tile(ASSET_PATH + "chars/W.png");
        gridState[0][7] = Tile(ASSET_PATH + "chars/N.png");
        gridState[0][8] = Tile(ASSET_PATH + "chars/D.png");
        gridState[0][9] = Tile(ASSET_PATH + "grid/PWND.png");
        gridState[0][10] = Tile(0.0f, 0.0f, 0.0f);
        gridState[0][11] = Tile(0.0f, 0.0f, 0.0f);
    }

    // SCORE AND MULTIPLIER DISPLAY

    // Update score tiles
    int score = gameState->getScore();
    for (int i = 9; i >= 4; --i) {
        gridState[14][i] = Tile(ASSET_PATH + "chars/" + std::to_string(score % 10) + ".png");
        score /= 10;
    }

    // Update multiplier tiles
    int multiplier = gameState->getMultiplier();
    gridState[14][11] = Tile(ASSET_PATH + "chars/" + std::to_string(multiplier / 10) + ".png");
    gridState[14][12] = Tile(ASSET_PATH + "chars/" + std::to_string(multiplier % 10) + ".png");

    // Update tally display in row 17
    int tally = gameState->getTally(); // Get the current tally

    // Update columns 4 to 8 with TALLY-ON1 or TALLY-OFF1
    for (int i = 4; i <= 8; ++i) {
        if (tally > 0) {
            gridState[17][i] = Tile(ASSET_PATH + "grid/TALLY-ON1.png");
            --tally;
        }
        else {
            gridState[17][i] = Tile(ASSET_PATH + "grid/TALLY-OFF1.png");
        }
    }

    // Update columns 9 to 12 with TALLY-ON2 or TALLY-OFF2
    for (int i = 9; i <= 12; ++i) {
        if (tally > 0) {
            gridState[17][i] = Tile(ASSET_PATH + "grid/TALLY-ON2.png");
            --tally;
        }
        else {
            gridState[17][i] = Tile(ASSET_PATH + "grid/TALLY-OFF2.png");
        }
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
