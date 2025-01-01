#include "main_menu.h"
#include "graphics.h"
#include "world.h"

MainMenu::MainMenu()
    : startButton(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 - 50, 200, 50, "Start", []() { /* Start game */ }),
    optionsButton(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 200, 50, "Options", []() { /* Go to Options Menu */ }),
    exitButton(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 + 50, 200, 50, "Exit", []() { graphics::closeWindow(); }) {}

void MainMenu::update() {
    if (startButton.isClicked()) {
        // Transition to the game
    }
    if (optionsButton.isClicked()) {
        // Transition to Options Menu
    }
    if (exitButton.isClicked()) {
        graphics::closeWindow();
    }
}

void MainMenu::draw() {
    graphics::Brush br;
    br.fill_color[0] = 0.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f; // Black background
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

    startButton.draw();
    optionsButton.draw();
    exitButton.draw();
}
