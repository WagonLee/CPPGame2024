#include "graphics.h"
#include "config.h"
#include "Menu.h"
#include "GameState.h" // To initialize the game
#include <string>
#include "MenuUtils.h"

void Menu::init() {
    graphics::setFont(ASSET_PATH + "Arial.ttf"); // Simplified asset path
}

// State to track key presses
bool keyUpPressed = false;
bool keyDownPressed = false;
bool keySelectPressed = false;

void Menu::update() {
    static bool selectTriggered = false;

    // Update menu selection using MAIN_MENU_OPTIONS
    selectedOption = handleMenuInput(MAIN_MENU_OPTIONS, selectedOption, selectTriggered);

    // Get game state instance
    GameState* gameState = GameState::getInstance();

    // Handle option selection
    if (selectTriggered) {
        switch (selectedOption) {
        case 0: // PLAY
            inMenu = false;
            gameState->resetGameStates();
            gameState->init();
            break;
        case 1: // HIGH SCORES
            // Implement high score logic
            break;
        case 2: // TUTORIAL
            // Implement tutorial logic
            break;
        case 3: // EXIT
            graphics::stopMessageLoop();
            break;
        }
    }
}

void Menu::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Draw background
    br.texture = ASSET_PATH + "menu_background.png"; // Simplified asset path
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

    // Draw menu options
    const float fontSize = 30.0f; // Font size
    const float spacing = 50.0f;  // Vertical spacing between options
    const float charWidth = 15.0f; // Approximate width per character for font size 30

    for (size_t i = 0; i < MAIN_MENU_OPTIONS.size(); ++i) {
        br.fill_color[0] = 1.0f; // Default white color
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;

        if (i == selectedOption) {
            br.fill_color[0] = 0.0f; // Highlighted color (green)
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;
        }

        // Calculate horizontal and vertical positioning
        float textWidth = MAIN_MENU_OPTIONS[i].size() * charWidth; // Approximate text width
        float x = (CANVAS_WIDTH - textWidth) / 2; // Center horizontally
        float y = (CANVAS_HEIGHT / 2) + (i - MAIN_MENU_OPTIONS.size() / 2.0f) * spacing; // Vertical alignment

        // Draw the menu option
        graphics::drawText(x, y, fontSize, MAIN_MENU_OPTIONS[i], br);
    }
}
