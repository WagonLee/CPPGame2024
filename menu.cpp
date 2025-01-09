#include "graphics.h"
#include "config.h"
#include "Menu.h"
#include <string>

void Menu::init() {
    graphics::setFont(std::string("assets/Arial.ttf")); // Simplified asset path
}

// State to track key presses
bool keyUpPressed = false;
bool keyDownPressed = false;

void Menu::update() {
    // Handle DOWN key with debounce
    if (graphics::getKeyState(graphics::SCANCODE_DOWN)) {
        if (!keyDownPressed) {
            selectedOption = (selectedOption + 1) % options.size(); // Move down
            keyDownPressed = true; // Mark key as pressed
        }
    }
    else {
        keyDownPressed = false; // Reset key state when released
    }

    // Handle UP key with debounce
    if (graphics::getKeyState(graphics::SCANCODE_UP)) {
        if (!keyUpPressed) {
            selectedOption = (selectedOption - 1 + options.size()) % options.size(); // Move up
            keyUpPressed = true; // Mark key as pressed
        }
    }
    else {
        keyUpPressed = false; // Reset key state when released
    }

    // Handle ENTER key
    if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {
        switch (selectedOption) {
        case 0:
            // Start the game (implement transition to gameplay)
            break;
        case 1:
            // Show high scores (implement transition to high scores screen)
            break;
        case 2:
            // Show tutorial (implement transition to tutorial screen)
            break;
        case 3:
            // Exit the game
            graphics::destroyWindow();
            break;
        }
    }
}

void Menu::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Draw background
    br.texture = "assets/menu_background.png"; // Simplified asset path
    graphics::drawRect(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT, br);

    // Draw menu options
    for (size_t i = 0; i < options.size(); ++i) {
        br.fill_color[0] = 1.0f; // Default white color
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;

        if (i == selectedOption) {
            br.fill_color[0] = 0.0f; // Highlighted color (green)
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;
        }

        float x = WINDOW_WIDTH / 2;
        float y = WINDOW_HEIGHT / 2 + (i - options.size() / 2) * 50;
        graphics::drawText(x - 50, y, 30, options[i], br);
    }
}
