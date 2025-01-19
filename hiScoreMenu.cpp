#include "HiScoreMenu.h"
#include "MainMenu.h"
#include "MenuUtils.h"
#include "graphics.h"
#include <iostream>
#include <algorithm>

// Static member definitions
HiScoreMenu* HiScoreMenu::instance = nullptr;
std::vector<int> HiScoreMenu::scores;
bool HiScoreMenu::defaultScoresInitialized = false;

HiScoreMenu::HiScoreMenu() {}

HiScoreMenu* HiScoreMenu::getInstance() {
    if (!instance) {
        instance = new HiScoreMenu();
    }
    return instance;
}

void HiScoreMenu::initDefaultScores() {
    if (!defaultScoresInitialized) {
        scores = {
            200,
            800,
            2000,
            5000,
            8000,
            15000,
            30000,
            50000,
            70000,
            100000
        };
        defaultScoresInitialized = true;
    }
}

void HiScoreMenu::init() {
    initDefaultScores(); // Ensure default scores are initialized once

    // Initialize the menu grid
    menuGridState = std::vector<std::vector<Tile>>(
        GRID_HEIGHT,
        std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f))
        );

    selectedOption = 0;
    debounceFrameCount = 0;

    std::cout << "HiScoreMenu initialized with grid size: "
        << GRID_HEIGHT << "x" << GRID_WIDTH << std::endl;
}

void HiScoreMenu::update(float dt) {
    if (debounceFrameCount < DEBOUNCE_FRAMES) {
        debounceFrameCount++;
        return;
    }

    static bool selectTriggered = false;

    selectedOption = handleMenuInput({ "BACK" }, selectedOption, selectTriggered);

    if (selectTriggered) {
        std::cout << "Exiting HiScoreMenu, transitioning to MainMenu" << std::endl;
        graphics::playSound(ASSET_PATH + "sounds/select.wav", 1.0f, false);
        inHiScores = false;
        inMenu = true;
        MainMenu::getInstance()->init();
    }
}

void HiScoreMenu::updateLeaderboard(int score) {
    if (score > 0) {
        scores.push_back(score);
        std::sort(scores.begin(), scores.end(), std::greater<int>());

        if (scores.size() > 10) {
            scores.resize(10);
        }
        std::cout << "Updated leaderboard with score: " << score << std::endl;
    }
}

void HiScoreMenu::draw() {
    // Debug: Print current scores
    std::cout << "Current scores in leaderboard: ";
    for (int score : scores) {
        std::cout << score << " ";
    }
    std::cout << std::endl;

    // Clear the grid first
    clearGrid();

    // Draw the title "HISCORES"
    const std::vector<std::string> title = { "H.png", "I.png", "S.png", "C.png", "O.png", "R.png", "E.png", "S.png" };
    drawTitle(title, 2);

    // Draw each score in 'scores' on a new row
    int row = 4;
    for (int s : scores) {
        if (row >= GRID_HEIGHT) break; // Safeguard against out-of-bounds

        // Convert score to textures
        std::string sStr = std::to_string(s);
        std::vector<std::string> digits;
        for (char c : sStr) {
            digits.push_back(std::string(1, c) + ".png");
        }

        drawOptions({ digits }, row);
        row += 1; // Move down one line per score
    }

    // Draw "BACK" option a bit lower
    row += 2;
    if (row < GRID_HEIGHT) {
        drawOptions({ {"B.png", "A.png", "C.png", "K.png"} }, row);
    }

    // Render the grid (replacement for renderGrid)
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