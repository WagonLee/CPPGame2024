#include "graphics.h"
#include "config.h"
#include "Menu.h"
#include "GameState.h" // To initialize the game
#include <string>
#include "MenuUtils.h"

void Menu::init() {
    // Initialize menu grid with default black tiles
    menuGridState = std::vector<std::vector<Tile>>(GRID_HEIGHT, std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f)));

    // Play menu music in a loop with reduced volume
    graphics::playMusic(ASSET_PATH + "sounds/MENU.mp3", 0.6f, true);
}

void Menu::update() {
    static bool selectTriggered = false;

    // Update menu selection using MAIN_MENU_OPTIONS
    int prevOption = selectedOption;
    selectedOption = handleMenuInput(MAIN_MENU_OPTIONS, selectedOption, selectTriggered);

    // Play sound for up/down navigation
    if (selectedOption != prevOption) {
        if (selectedOption > prevOption) {
            graphics::playSound(ASSET_PATH + "sounds/down.wav", 1.0f, false);
        }
        else {
            graphics::playSound(ASSET_PATH + "sounds/up.wav", 1.0f, false);
        }
    }

    // Get game state instance
    GameState* gameState = GameState::getInstance();

    // Handle option selection
    if (selectTriggered) {
        graphics::playSound(ASSET_PATH + "sounds/select.wav", 1.0f, false); // Play select sound

        switch (selectedOption) {
        case 0: // PLAY
            graphics::stopMusic(); // Stop the menu music
            inMenu = false;
            gameState->resetGameStates();
            gameState->init();
            break;
        case 1: // HIGH SCORES
            // Implement high score logic
            break;
        case 2: // HELP
            // Implement help logic
            break;
        case 3: // EXIT
            graphics::stopMessageLoop();
            break;
        }
    }
}

void Menu::draw() {
    // Clear the menu grid to black
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            menuGridState[row][col] = Tile(0.0f, 0.0f, 0.0f); // Black background
        }
    }

    // Add the game title "BYTERAIDER"
    const std::vector<std::string> gameTitle = { "B.png", "Y.png", "T.png", "E.png", "R.png", "A.png", "I.png", "D.png", "E.png", "R.png" };
    int titleStartCol = (GRID_WIDTH - static_cast<int>(gameTitle.size())) / 2; // Center title
    int titleRow = 2; // Row to place the title
    for (const auto& charTexture : gameTitle) {
        menuGridState[titleRow][titleStartCol].texture = ASSET_PATH + "chars/" + charTexture;
        menuGridState[titleRow][titleStartCol] = Tile(1.0f, 1.0f, 1.0f); // White text
        ++titleStartCol;
    }

    // Define menu options using textures
    const std::vector<std::vector<std::string>> menuOptions = {
        {"P.png", "L.png", "A.png", "Y.png"},        // PLAY
        {"H.png", "I.png", "S.png", "C.png", "O.png", "R.png", "E.png", "S.png"}, // HISCORES
        {"H.png", "E.png", "L.png", "P.png"},       // HELP
        {"E.png", "X.png", "I.png", "T.png"}        // EXIT
    };

    // Draw each menu option on the menu grid
    int startRow = 6; // Starting row for menu options
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        int startCol = (GRID_WIDTH - static_cast<int>(menuOptions[i].size())) / 2; // Center text
        for (const auto& charTexture : menuOptions[i]) {
            if (i == selectedOption) {
                // Highlight the selected option with a green background
                menuGridState[startRow][startCol] = Tile(0.0f, 1.0f, 0.0f); // Green highlight
            }
            else {
                // Non-selected text is white
                menuGridState[startRow][startCol] = Tile(1.0f, 1.0f, 1.0f);
            }
            menuGridState[startRow][startCol].texture = ASSET_PATH + "chars/" + charTexture;
            ++startCol;
        }
        startRow += 2; // Add 1 grid tall space between each option
    }

    // Render the menu grid
    graphics::Brush br;
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            const auto& tile = menuGridState[row][col];
            br.fill_color[0] = tile.r;
            br.fill_color[1] = tile.g;
            br.fill_color[2] = tile.b;

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
