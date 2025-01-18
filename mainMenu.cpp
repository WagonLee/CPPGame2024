#include "MainMenu.h"
#include "MenuUtils.h"
#include "graphics.h"
#include "GameState.h"
#include "HiScoreMenu.h"

// Global flags from main.cpp
extern bool inMenu;
extern bool inHiScores;

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu() {}

MainMenu* MainMenu::getInstance() {
    if (!instance) {
        instance = new MainMenu();
    }
    return instance;
}

void MainMenu::init() {
    selectedOption = 0;
    currCooldown = 0.0f;

    // Initialize your grid, etc.
    menuGridState = std::vector<std::vector<Tile>>(
        GRID_HEIGHT,
        std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f))
        );

    graphics::playMusic(ASSET_PATH + "sounds/MENU.mp3", 0.6f, true);
}

void MainMenu::update(float dt) {
    static bool selectTriggered = false;

    int prevOption = selectedOption;
    selectedOption = handleMenuInput(MAIN_MENU_OPTIONS, selectedOption, selectTriggered);

    if (selectedOption != prevOption) {
        // Play up/down feedback
        graphics::playSound(
            ASSET_PATH + (selectedOption > prevOption ? "sounds/down.wav" : "sounds/up.wav"),
            0.5f,
            false
        );
    }

    if (selectTriggered) {
        // The user pressed Return this frame
        graphics::playSound(ASSET_PATH + "sounds/select.wav", 0.5f, false);

        switch (selectedOption) {
        case 0:
            // PLAY
            graphics::stopMusic();
            inMenu = false;
            GameState::getInstance()->resetGameStates();
            GameState::getInstance()->init();
            break;

        case 1:
            // HISCORES
            inMenu = false;
            inHiScores = true;
            HiScoreMenu::getInstance()->init();
            break;

        case 2:
            // HELP
            // (not implemented)
            break;

        case 3:
            // EXIT
            graphics::stopMessageLoop();
            break;
        }
    }
}

void MainMenu::draw() {
    clearGrid();

    // Title
    const std::vector<std::string> title = {
        "B.png", "Y.png", "T.png", "E.png", "R.png",
        "A.png", "I.png", "D.png", "E.png", "R.png"
    };
    drawTitle(title, 3);

    // Menu Options
    const std::vector<std::vector<std::string>> menuOptions = {
        {"P.png","L.png","A.png","Y.png"},
        {"H.png","I.png","S.png","C.png","O.png","R.png","E.png","S.png"},
        {"H.png","E.png","L.png","P.png"},
        {"E.png","X.png","I.png","T.png"}
    };
    drawOptions(menuOptions, 6);

    // Render
    graphics::Brush br;
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            const auto& tile = menuGridState[row][col];
            br.fill_color[0] = tile.r;
            br.fill_color[1] = tile.g;
            br.fill_color[2] = tile.b;
            br.texture = tile.texture.empty() ? "" : tile.texture;
            br.outline_opacity = 0.0f;

            float x = col * CELL_SIZE + CELL_SIZE / 2;
            float y = row * CELL_SIZE + CELL_SIZE / 2;
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, br);
        }
    }
}
