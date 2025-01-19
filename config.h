#pragma once

// Asset directory path
const std::string ASSET_PATH = "bin/assets/";

// Game state flags 
extern bool inMenu;
extern bool inHiScores;
extern bool inTutorial;

// Grid layout constants
const int GRID_WIDTH = 14;
const int PLAYABLE_COLUMNS = 12;
const int PLAYABLE_ROWS = 12;
const int UI_ROWS_ABOVE = 1;
const int UI_ROWS_BELOW = 6;
const int GRID_HEIGHT = PLAYABLE_ROWS + UI_ROWS_ABOVE + UI_ROWS_BELOW;

// Canvas dimensions and scaling
const float CANVAS_WIDTH = 900.0f;
const float CELL_SIZE = CANVAS_WIDTH / GRID_WIDTH;
const float CANVAS_HEIGHT = GRID_HEIGHT * CELL_SIZE;