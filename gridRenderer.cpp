#include "GridRenderer.h"
#include "graphics.h"
#include "config.h" 

// Function to draw the game grid and borders
void drawGrid() {
    graphics::Brush br;

    // Loop through all grid cells
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {

            // --- Handle Specific Tile Types ---

            // Top-most row (individually addressable tiles)

            if (row == 0) {
                if (col == 0) {
                    // Top-left corner of the window (Red)
                    br.fill_color[0] = 1.0f; // Red
                    br.fill_color[1] = 0.0f;
                    br.fill_color[2] = 0.0f;
                }
                else if (col == 1) {
                    // Tile at (0, 1)
                    br.fill_color[0] = 0.9f; // Light red
                    br.fill_color[1] = 0.1f;
                    br.fill_color[2] = 0.1f;
                }
                else if (col == 2) {
                    // Tile at (0, 2)
                    br.fill_color[0] = 0.8f;
                    br.fill_color[1] = 0.2f;
                    br.fill_color[2] = 0.2f;
                }
                else if (col == 3) {
                    // Tile at (0, 3)
                    br.fill_color[0] = 0.7f;
                    br.fill_color[1] = 0.3f;
                    br.fill_color[2] = 0.3f;
                }
                else if (col == 4) {
                    // Tile at (0, 4)
                    br.fill_color[0] = 0.6f;
                    br.fill_color[1] = 0.4f;
                    br.fill_color[2] = 0.4f;
                }
                else if (col == 5) {
                    // Tile at (0, 5)
                    br.fill_color[0] = 0.5f;
                    br.fill_color[1] = 0.5f;
                    br.fill_color[2] = 0.5f;
                }
                else if (col == 6) {
                    // Tile at (0, 6)
                    br.fill_color[0] = 0.4f;
                    br.fill_color[1] = 0.6f;
                    br.fill_color[2] = 0.6f;
                }
                else if (col == 7) {
                    // Tile at (0, 7)
                    br.fill_color[0] = 0.3f;
                    br.fill_color[1] = 0.7f;
                    br.fill_color[2] = 0.7f;
                }
                else if (col == 8) {
                    // Tile at (0, 8)
                    br.fill_color[0] = 0.2f;
                    br.fill_color[1] = 0.8f;
                    br.fill_color[2] = 0.8f;
                }
                else if (col == 9) {
                    // Tile at (0, 9)
                    br.fill_color[0] = 0.1f;
                    br.fill_color[1] = 0.9f;
                    br.fill_color[2] = 0.9f;
                }
                else if (col == 10) {
                    // Tile at (0, 10)
                    br.fill_color[0] = 0.2f; // Cyan-like placeholder
                    br.fill_color[1] = 0.9f;
                    br.fill_color[2] = 0.1f;
                }
                else if (col == 11) {
                    // Tile at (0, 11)
                    br.fill_color[0] = 0.3f; // Cyan-like placeholder
                    br.fill_color[1] = 0.2f;
                    br.fill_color[2] = 0.9f;
                }
                else if (col == GRID_WIDTH - 2) {
                    // Second to last tile in the top row
                    br.fill_color[0] = 0.2f;
                    br.fill_color[1] = 0.2f;
                    br.fill_color[2] = 0.7f;
                }
                else if (col == GRID_WIDTH - 1) {
                    // Top-right corner of the window (Green)
                    br.fill_color[0] = 0.0f; // Green
                    br.fill_color[1] = 1.0f;
                    br.fill_color[2] = 0.0f;
                }
            }

            // 2. Corners of the entire window (excluding top row tiles)
            else if (row == GRID_HEIGHT - 1 && col == 0) {
                // Bottom-left corner of the window (Blue)
                br.fill_color[0] = 1.0f;  // Orange
                br.fill_color[1] = 0.5f;
                br.fill_color[2] = 0.0f;
            }
            else if (row == GRID_HEIGHT - 1 && col == GRID_WIDTH - 1) {
                // Bottom-right corner of the window (Yellow)
                br.fill_color[0] = 1.0f; // Yellow
                br.fill_color[1] = 1.0f;
                br.fill_color[2] = 0.0f;
            }

            // 3. Bottom corners of the grid border
            else if (row == UI_ROWS_ABOVE + PLAYABLE_ROWS && col == 0) {
                // Bottom-left corner of the grid border (Cyan)
                br.fill_color[0] = 0.0f; // Cyan
                br.fill_color[1] = 1.0f;
                br.fill_color[2] = 1.0f;
            }
            else if (row == UI_ROWS_ABOVE + PLAYABLE_ROWS && col == PLAYABLE_COLUMNS + 1) {
                // Bottom-right corner of the grid border (Pink)
                br.fill_color[0] = 1.0f; // Pink
                br.fill_color[1] = 0.0f;
                br.fill_color[2] = 1.0f;
            }

            // 4. Left and right border columns (Blue)
            else if (col == 0 || col == GRID_WIDTH - 1) {
                br.fill_color[0] = 0.2f; // Blue
                br.fill_color[1] = 0.0f;
                br.fill_color[2] = 0.5f;
            }

            // 5. Rows above and below the playable grid (Blue)
            else if (row < UI_ROWS_ABOVE || row >= UI_ROWS_ABOVE + PLAYABLE_ROWS) {
                br.fill_color[0] = 0.0f; // Blue
                br.fill_color[1] = 0.5f;
                br.fill_color[2] = 1.0f;
            }

            // 6. Playable rows (Alternating Black and White)
            else {
                if ((row + col) % 2 == 0) {
                    br.fill_color[0] = 0.0f; // Black
                    br.fill_color[1] = 0.0f;
                    br.fill_color[2] = 0.0f;
                }
                else {
                    br.fill_color[0] = 1.0f; // White
                    br.fill_color[1] = 1.0f;
                    br.fill_color[2] = 1.0f;
                }
            }

            // --- Draw the Tile ---

            // Set solid fill and remove all outlines
            br.fill_opacity = 1.0f;
            br.outline_opacity = 0.0f; // Completely remove outlines

            // Calculate the position
            float x = col * CELL_SIZE + CELL_SIZE / 2;
            float y = row * CELL_SIZE + CELL_SIZE / 2;

            // Draw the cell
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, br);
        }
    }
}