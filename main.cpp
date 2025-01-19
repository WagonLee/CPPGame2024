#include "graphics.h"
#include "gamestate.h"
#include "mainmenu.h"
#include "hiscoremenu.h"
#include "tutorialmenu.h"
#include "gridrenderer.h"
#include <iostream>
#include "config.h"

bool inMenu = true;
bool inHiScores = false;
bool inTutorial = false;

// Menu instances
MainMenu* mainMenu = MainMenu::getInstance();
HiScoreMenu* hiScoreMenu = HiScoreMenu::getInstance();
TutorialMenu* tutorialMenu = TutorialMenu::getInstance();

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
        GameState::getInstance()->draw();
    }
}

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
        GameState::getInstance()->update(dt);
        updateGrid(); // Dynamically update tiles (score, kills, etc.)
    }
}

int main() {
    graphics::createWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "BYTERAIDER");

    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    initGrid(); 

    mainMenu->init();

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    // Start the game loop
    graphics::startMessageLoop();

    graphics::destroyWindow();
    return 0;
}
