#include "Menu.h"

void Menu::clearGrid() {
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            menuGridState[row][col] = Tile(0.0f, 0.0f, 0.0f); // Black background
            menuGridState[row][col].texture = ""; // Clear textures
        }
    }
}

void Menu::drawTitle(const std::vector<std::string>& title, int row) {
    // Calculate center
    int startCol = (GRID_WIDTH - static_cast<int>(title.size())) / 2;

    // Draw each character
    for (size_t i = 0; i < title.size(); ++i) {
        // Assign white tile
        menuGridState[row][startCol + i] = Tile(1.0f, 1.0f, 1.0f);
        // Then set texture
        menuGridState[row][startCol + i].texture = ASSET_PATH + "chars/" + title[i];
    }
}

void Menu::drawOptions(const std::vector<std::vector<std::string>>& options, int startRow) {
    for (size_t i = 0; i < options.size(); ++i) {
        // Calculate center for this option
        int startCol = (GRID_WIDTH - static_cast<int>(options[i].size())) / 2;

        // Draw each character in this option
        for (size_t j = 0; j < options[i].size(); ++j) {
            // Highlight if selected
            if (i == selectedOption) {
                menuGridState[startRow][startCol + j] = Tile(0.0f, 1.0f, 0.0f);
            }
            else {
                menuGridState[startRow][startCol + j] = Tile(1.0f, 1.0f, 1.0f);
            }
            // Set texture
            menuGridState[startRow][startCol + j].texture = ASSET_PATH + "chars/" + options[i][j];
        }
        // Move down a couple of rows for spacing
        startRow += 2;
    }
}
