#include "HiScoreMenu.h"
#include "MainMenu.h"
#include "MenuUtils.h"
#include "graphics.h"
#include <cassert> // For debugging assertions
#include <iostream> // For debug output

HiScoreMenu* HiScoreMenu::instance = nullptr;

HiScoreMenu::HiScoreMenu() {}

HiScoreMenu* HiScoreMenu::getInstance() {
    if (!instance) {
        instance = new HiScoreMenu();
    }
    return instance;
}

void HiScoreMenu::init() {
    scores = { 300000, 270000, 250000, 230000, 210000, 190000, 170000, 150000, 130000, 2000 };
    menuGridState = std::vector<std::vector<Tile>>(GRID_HEIGHT, std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f)));
    selectedOption = 0;

    // Add this to prevent immediate trigger
    inputCooldown = 0.2f;  // Add this as a member variable
    lastInputTime = 0.0f;  // Add this as a member variable
}

void HiScoreMenu::update(float dt) {
    static bool selectTriggered = false;

    // Add input cooldown
    lastInputTime += dt;
    if (lastInputTime < inputCooldown) {
        return;  // Skip input processing during cooldown
    }

    selectedOption = handleMenuInput({ "BACK" }, selectedOption, selectTriggered);

    if (selectTriggered) {
        graphics::playSound(ASSET_PATH + "sounds/select.wav", 1.0f, false);
        inHiScores = false;
        inMenu = true;
        MainMenu::getInstance()->init();
    }
}

void HiScoreMenu::draw() {
    clearGrid();

    //// Draw Title
    //const std::vector<std::string> title = { "H.png", "I.png", "-", "S.png", "C.png", "O.png", "R.png", "E.png", "S.png" };
    //int titleRow = 2;
    //int titleStartCol = (GRID_WIDTH - static_cast<int>(title.size())) / 2;

    //assert(titleStartCol >= 0 && titleStartCol + title.size() <= GRID_WIDTH); // Debug
    //drawTitle(title, titleRow);

    //// Draw Scores
    //int startRow = 4;
    //for (const auto& score : scores) {
    //    std::vector<std::string> scoreTextures;
    //    for (char c : std::to_string(score)) {
    //        scoreTextures.push_back(std::string(1, c) + ".png"); // Convert each char to texture
    //    }

    //    assert(startRow >= 0 && startRow < GRID_HEIGHT); // Debug
    //    drawOptions({ scoreTextures }, startRow);
    //    startRow += 2; // Move down for next score
    //}

    //// Draw "BACK" button
    //const std::vector<std::string> backOption = { "B.png", "A.png", "C.png", "K.png" };
    //int backRow = startRow + 2;

    //assert(backRow >= 0 && backRow < GRID_HEIGHT); // Debug
    //drawOptions({ backOption }, backRow);
}
