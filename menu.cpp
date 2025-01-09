#include "graphics.h"
#include "config.h"
#include "Menu.h"
#include "GameState.h" // To initialize the game
#include <string>

void Menu::init() {
    graphics::setFont(std::string("assets/Arial.ttf")); // Simplified asset path
}

// State to track key presses
bool keyUpPressed = false;
bool keyDownPressed = false;
bool keySelectPressed = false;

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

    // Handle selection with ENTER or SPACE
    if (graphics::getKeyState(graphics::SCANCODE_RETURN) || graphics::getKeyState(graphics::SCANCODE_SPACE)) {
        if (!keySelectPressed) {
            switch (selectedOption) {
            case 0: { // PLAY
                // Properly transition to gameplay
                extern bool inMenu; // Access the global menu flag
                inMenu = false;     // Exit the menu

                GameState* gameState = GameState::getInstance();
                gameState->resetGameStates(); // Reset game state flags
                gameState->init();            // Initialize the game
                break;
            }
            case 1: // HIGH SCORES
                // Implement high score logic
                break;
            case 2: // TUTORIAL
                // Implement tutorial logic
                break;
            case 3: // EXIT
                graphics::stopMessageLoop(); // Gracefully stop the SGG message loop
                break;
            }
            keySelectPressed = true; // Mark key as pressed
        }
    }
    else {
        keySelectPressed = false; // Reset key state when released
    }
}

void Menu::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Draw background
    br.texture = "assets/menu_background.png"; // Simplified asset path
    graphics::drawRect(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT, br);

    // Draw menu options
    const float charWidth = 15.0f; // Approximate width per character for size 30 font
    for (size_t i = 0; i < options.size(); ++i) {
        br.fill_color[0] = 1.0f; // Default white color
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;

        if (i == selectedOption) {
            br.fill_color[0] = 0.0f; // Highlighted color (green)
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;
        }

        float textWidth = options[i].size() * charWidth; // Approximate text width
        float x = (WINDOW_WIDTH - textWidth) / 2; // Center horizontally
        float y = WINDOW_HEIGHT / 2 + (i - options.size() / 2) * 50; // Vertical alignment

        graphics::drawText(x, y, 30, options[i], br);
    }
}
