#include "HiScoreMenu.h"
#include "MainMenu.h"
#include "MenuUtils.h"
#include "graphics.h"

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

    // Initialize the menu grid
    menuGridState = std::vector<std::vector<Tile>>(GRID_HEIGHT, std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f)));
}

void HiScoreMenu::update(float dt) {
    static bool selectTriggered = false;

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
    drawTitle({ "H.png", "I.png", "-", "S.png", "C.png", "O.png", "R.png", "E.png", "S.png" }, 2);

    int startRow = 4;
    for (const auto& score : scores) {
        std::vector<std::string> scoreTextures;
        for (char c : std::to_string(score)) {
            scoreTextures.push_back(std::string(1, c) + ".png"); // Convert each char to a string
        }
        drawOptions({ scoreTextures }, startRow); // Render the score row
        startRow += 2;
    }

    drawOptions({ {"B.png", "A.png", "C.png", "K.png"} }, startRow + 2); // Render "BACK"
}
