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
        inMenu = true;
        MainMenu::getInstance()->init();
    }
}

void TutorialMenu::draw() {
    // 1) Draw the background image first
    {
        graphics::Brush bg;
        bg.texture = ASSET_PATH + "test.png";
        bg.outline_opacity = 0.0f;

        float w = GRID_WIDTH * CELL_SIZE;
        float h = (GRID_HEIGHT - 2) * CELL_SIZE; // space for title/back
        float x = (GRID_WIDTH * CELL_SIZE) / 2.0f;
        float y = (GRID_HEIGHT * CELL_SIZE) / 2.0f;
        graphics::drawRect(x, y, w, h, bg);
    }

    // 2) Clear the grid (which internally sets every tile's color/texture to blank)
    clearGrid();

    // 3) Draw your title and "BACK" the usual way
    drawTitle({ "H.png", "E.png", "L.png", "P.png" }, 2);

    int backRow = GRID_HEIGHT - 2;
    drawOptions({ {"B.png", "A.png", "C.png", "K.png"} }, backRow);

    // 4) Render only the tiles that have a texture (text). 
    //    Skip empty tiles so the background remains visible.
    graphics::Brush br;
    for (int r = 0; r < GRID_HEIGHT; ++r) {
        for (int c = 0; c < GRID_WIDTH; ++c) {
            const auto& tile = menuGridState[r][c];

            // If this tile has no texture, do nothing (keeps background visible)
            if (tile.texture.empty())
                continue;

            // Otherwise, draw the textured tile (letter)
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
