#pragma once

const std::string ASSET_PATH = "bin/assets/";

// Global toggle for menu state
extern bool inMenu; // Declare inMenu as external
extern bool inHiScores; // Tracks if the hiScore menu is active

// Grid Dimensions
const int GRID_WIDTH = 14;       // Increased by 2 for left and right columns
const int PLAYABLE_COLUMNS = 12; // Still 12 playable columns
const int PLAYABLE_ROWS = 12;    // Rows accessible to the player
const int UI_ROWS_ABOVE = 1;     // Rows above the playable grid (for UI or flavor)
const int UI_ROWS_BELOW = 6;     // Rows below the playable grid (for UI or flavor)
const int GRID_HEIGHT = PLAYABLE_ROWS + UI_ROWS_ABOVE + UI_ROWS_BELOW; // Total rows

// Canvas Dimensions
const float CANVAS_WIDTH = 900.0f;          // Logical canvas width
const float CELL_SIZE = CANVAS_WIDTH / GRID_WIDTH; // Consistent cell size
const float CANVAS_HEIGHT = GRID_HEIGHT * CELL_SIZE; // Logical canvas height based on rows
