#include "HiScoreMenu.h"
#include "MainMenu.h"
#include "MenuUtils.h"
#include "graphics.h"
#include <iostream> // For debug output

// Global flags from main.cpp
extern bool inMenu;
extern bool inHiScores;

HiScoreMenu* HiScoreMenu::instance = nullptr;

HiScoreMenu::HiScoreMenu() {}

HiScoreMenu* HiScoreMenu::getInstance() {
    if (!instance) {
        instance = new HiScoreMenu();
    }
    return instance;
}

void HiScoreMenu::init() {
    // Example scores
    scores = {
    2000,    
    4000,
    7000,
    12000,
    50000,
    100000,  
    150000,
    200000,
    250000,
    300000   
    };

    // Initialize the menu grid
    menuGridState = std::vector<std::vector<Tile>>(
        GRID_HEIGHT,
        std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f))
        );

    selectedOption = 0; // Always start with the first option

    // Reset debounce
    debounceFrameCount = 0;

    std::cout << "HiScoreMenu initialized with grid size: "
        << GRID_HEIGHT << "x" << GRID_WIDTH << std::endl;
}

void HiScoreMenu::update(float dt) {
    // Skip input for a few frames so we don't immediately detect leftover Enter presses
    if (debounceFrameCount < DEBOUNCE_FRAMES) {
        debounceFrameCount++;
        return;
    }

    static bool selectTriggered = false;

    // We only have 1 logical option: "BACK". 
    // So handleMenuInput can move selectedOption around, but it will always be 0 if clamped.
    selectedOption = handleMenuInput({ "BACK" }, selectedOption, selectTriggered);

    if (selectTriggered) {
        std::cout << "Exiting HiScoreMenu, transitioning to MainMenu" << std::endl;
        graphics::playSound(ASSET_PATH + "sounds/select.wav", 1.0f, false);

        inHiScores = false;
        inMenu = true;
        MainMenu::getInstance()->init();
    }
}

void HiScoreMenu::draw()
{
    // 1) Clear the grid first
    clearGrid();

    // 2) Draw the title "HISCORES"
    //    (Make sure you have H.png, I.png, S.png, C.png, O.png, R.png, E.png, S.png in your chars folder)
    const std::vector<std::string> title = {
        "H.png","I.png","S.png","C.png","O.png","R.png","E.png","S.png"
    };
    drawTitle(title, /*row*/ 2);

    // 3) Draw each score in 'scores' on a new row
    //    Start from row=4, increment by 1 each time so we don't run off the bottom
    int row = 4;
    for (int s : scores) {
        if (row >= GRID_HEIGHT) break; // safeguard against out-of-bounds

        // Convert score to textures, e.g. "3.png","0.png","0.png"...
        std::string sStr = std::to_string(s);
        std::vector<std::string> digits;
        for (char c : sStr) {
            digits.push_back(std::string(1, c) + ".png");
        }

        drawOptions({ digits }, row);
        row += 1; // move down one line per score
    }

    // 4) Draw "BACK" option a bit lower
    row += 2;
    if (row < GRID_HEIGHT) {
        std::vector<std::vector<std::string>> backOption = {
            {"B.png","A.png","C.png","K.png"}
        };
        drawOptions(backOption, row);
    }

    // 5) Render the grid
    graphics::Brush br;
    for (int r = 0; r < GRID_HEIGHT; ++r) {
        for (int c = 0; c < GRID_WIDTH; ++c) {
            const auto& tile = menuGridState[r][c];
            br.fill_color[0] = tile.r;
            br.fill_color[1] = tile.g;
            br.fill_color[2] = tile.b;
            br.texture = tile.texture.empty() ? "" : tile.texture;
            br.outline_opacity = 0.0f;

            float x = c * CELL_SIZE + CELL_SIZE / 2;
            float y = r * CELL_SIZE + CELL_SIZE / 2;
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, br);
        }
    }
}
