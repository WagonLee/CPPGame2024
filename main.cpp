#include "graphics.h"
#include "GameState.h"
#include "MainMenu.h"
#include "HiScoreMenu.h"
#include "tutorialMenu.h"
#include "GridRenderer.h"
#include <iostream>
#include "config.h"

// Global state flags
bool inMenu = true;   // Defaults to main menu
bool inHiScores = false;
bool inTutorial = false;

// Menu instances
MainMenu* mainMenu = MainMenu::getInstance();
HiScoreMenu* hiScoreMenu = HiScoreMenu::getInstance();
TutorialMenu* tutorialMenu = TutorialMenu::getInstance();

// Draw function
void draw() {
    if (inMenu) {
        mainMenu->draw();
    }
    else if (inHiScores) {
        hiScoreMenu->draw();
    }
    else if (inTutorial) {
        tutorialMenu->draw();
    }
    else {
        drawGrid();                       
        // Now draw the actual game objects
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
    else if (inTutorial) {
        tutorialMenu->update(dt);
    }
    else {
        // GAME branch: Update the game logic, then the grid
        GameState::getInstance()->update(dt);
        updateGrid(); // Dynamically update tiles (score, kills, etc.)
    }
}

int main() {
    // Create the window with default size
    graphics::createWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "BYTERAIDER");

    // Set logical canvas size and scaling mode
    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    initGrid(); // Initialize the grid for menu usage

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
