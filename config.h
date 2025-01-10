#pragma once

// Global toggle for menu state
extern bool inMenu; // Declare inMenu as external

// Grid and Cell Size
const int GRID_SIZE = 12;                    // 12x12 grid
const float CANVAS_WIDTH = 600.0f;           // Logical canvas width
const float CANVAS_HEIGHT = 600.0f;          // Logical canvas height
const float CELL_SIZE = CANVAS_WIDTH / GRID_SIZE; // Fixed cell size based on canvas dimensions

// Window Size (Optional: Match Canvas Dimensions)
const int WINDOW_WIDTH = 600;                // Default window width
const int WINDOW_HEIGHT = 600;               // Default window height
