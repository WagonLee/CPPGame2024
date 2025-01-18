#include "graphics.h"
#include "GameState.h"
#include "MainMenu.h"
#include "HiScoreMenu.h"
#include "GridRenderer.h"
#include <iostream> // For debug output
#include "tutorialMenu.h"

// Global state flags
bool inMenu = true;        // Default menu state
bool inHiScores = false;   // HiScore menu state

// Menu instances
MainMenu* mainMenu = MainMenu::getInstance();
HiScoreMenu* hiScoreMenu = HiScoreMenu::getInstance();

// Draw function
void draw() {
    if (inMenu) {
        mainMenu->draw();
    }
    else if (inHiScores) {
        hiScoreMenu->draw();
    }
    else if (!inMenu && !inHiScores) { // Tutorial Menu
        TutorialMenu::getInstance()->draw();
    }
    else {
        GameState::getInstance()->draw();
    }
}

// Update function
void update(float dt) {
    if (inMenu) {
        mainMenu->update(dt);
    }
    else if (inHiScores) {
        hiScoreMenu->update(dt);
    }
    else if (!inMenu && !inHiScores) { // Tutorial Menu
        TutorialMenu::getInstance()->update(dt);
    }
    else {
        GameState::getInstance()->update(dt);
    }
}

// Main function
int main() {
    // Create the window with default size
    graphics::createWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "BYTERAIDER");

    // Set logical canvas size and scaling mode
    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);       // Logical size
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);   // Maintain aspect ratio

    initGrid(); // Initialize the grid

    // Initialize the main menu
    mainMenu->init();

    // Set callbacks for drawing and updating
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    // Start the game loop
    graphics::startMessageLoop();

    // Destroy the window on exit
    graphics::destroyWindow();

    return 0;
}
