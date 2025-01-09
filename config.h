#pragma once

extern bool inMenu; // Declare inMenu as external

// Grid and Cell Size
const int GRID_SIZE = 12;              // 12x12 grid
const float CELL_SIZE = 50.0f;         // Fixed 50 pixels per cell

// Window Size
const int WINDOW_WIDTH = GRID_SIZE * CELL_SIZE;  // 600px (12 * 50)
const int WINDOW_HEIGHT = GRID_SIZE * CELL_SIZE; // 600px (12 * 50)
