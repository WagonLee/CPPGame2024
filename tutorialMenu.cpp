#include "TutorialMenu.h"
#include "MainMenu.h"
#include "graphics.h"
#include <cassert>
#include <iostream>
#include "MenuUtils.h"

TutorialMenu* TutorialMenu::instance = nullptr;

TutorialMenu::TutorialMenu() {}

TutorialMenu* TutorialMenu::getInstance() {
    if (!instance) {
        instance = new TutorialMenu();
    }
    return instance;
}

void TutorialMenu::init() {
    // Initialize the grid
    menuGridState = std::vector<std::vector<Tile>>(GRID_HEIGHT, std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f)));

    selectedOption = 0; // Start with the "BACK" option
    std::cout << "TutorialMenu initialized with grid size: " << GRID_HEIGHT << "x" << GRID_WIDTH << std::endl;
}

void TutorialMenu::update(float dt) {
    static bool selectTriggered = false;

    // Handle "BACK" option
    selectedOption = handleMenuInput({ "BACK" }, selectedOption, selectTriggered);

    if (selectTriggered) {
        std::cout << "Exiting TutorialMenu, transitioning to MainMenu" << std::endl;
        graphics::playSound(ASSET_PATH + "sounds/select.wav", 1.0f, false);
        inHiScores = false;
        inTutorial = false;
        inMenu = true;
        MainMenu::getInstance()->init();
    }
}

void TutorialMenu::draw() {
    // 1) Draw the background image for the entire menu
    {
        graphics::Brush bg;
        bg.texture = ASSET_PATH + "HELP.png";
        bg.outline_opacity = 0.0f;

        // Define smaller dimensions for the middle image
        float w = GRID_WIDTH * CELL_SIZE * 0.9f; // 
        float h = (GRID_HEIGHT - 6) * CELL_SIZE * 0.9f; 
        float x = (GRID_WIDTH * CELL_SIZE) / 2.0f; // Center horizontally
        float y = (GRID_HEIGHT * CELL_SIZE) / 2.0f; // Center vertically

        graphics::drawRect(x, y, w, h, bg); // Render the image
    }

    // 2) Clear the grid for the text elements
    clearGrid();

    // 3) Draw the title "HELP" at the top
    drawTitle({ "H.png", "E.png", "L.png", "P.png" }, 2);

    // 4) Draw the "BACK" button at the bottom
    int backRow = GRID_HEIGHT - 2;
    drawOptions({ {"B.png", "A.png", "C.png", "K.png"} }, backRow);

    // 5) Render the grid for text tiles
    graphics::Brush br;
    for (int r = 0; r < GRID_HEIGHT; ++r) {
        for (int c = 0; c < GRID_WIDTH; ++c) {
            const auto& tile = menuGridState[r][c];

            // Skip tiles without textures
            if (tile.texture.empty()) continue;

            br.fill_color[0] = tile.r;
            br.fill_color[1] = tile.g;
            br.fill_color[2] = tile.b;
            br.texture = tile.texture;
            br.outline_opacity = 0.0f;

            float x = c * CELL_SIZE + CELL_SIZE / 2;
            float y = r * CELL_SIZE + CELL_SIZE / 2;
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, br);
        }
    }
}
