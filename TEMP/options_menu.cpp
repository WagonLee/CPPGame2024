#include "options_menu.h"
#include "graphics.h"

OptionsMenu::OptionsMenu()
    : backButton(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 200, 50, "Back", []() { /* Go back to Main Menu */ }) {}

void OptionsMenu::update() {
    if (backButton.isClicked()) {
        // Transition to Main Menu
    }
}

void OptionsMenu::draw() {
    graphics::Brush br;
    br.fill_color[0] = 0.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f; // Black background
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

    backButton.draw();
}
