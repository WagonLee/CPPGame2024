#include "graphics.h"
#include "MenuUtils.h"

int handleMenuInput(const std::vector<std::string>& options, int currentSelection, bool& selectTriggered) {
    static bool keyUpPressed = false;
    static bool keyDownPressed = false;

    // Handle DOWN key with debounce
    if (graphics::getKeyState(graphics::SCANCODE_DOWN)) {
        if (!keyDownPressed) {
            currentSelection = (currentSelection + 1) % options.size(); // Move down
            keyDownPressed = true; // Mark key as pressed
        }
    }
    else {
        keyDownPressed = false; // Reset key state when released
    }

    // Handle UP key with debounce
    if (graphics::getKeyState(graphics::SCANCODE_UP)) {
        if (!keyUpPressed) {
            currentSelection = (currentSelection - 1 + options.size()) % options.size(); // Move up
            keyUpPressed = true; // Mark key as pressed
        }
    }
    else {
        keyUpPressed = false; // Reset key state when released
    }

    // Handle selection with ENTER
    if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {
        if (!selectTriggered) {
            selectTriggered = true; // Mark ENTER as triggered
        }
    }
    else {
        selectTriggered = false; // Reset ENTER state when released
    }

    return currentSelection;
}
