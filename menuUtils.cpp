#include "graphics.h"
#include "MenuUtils.h"
#include <iostream> // For debug output

int handleMenuInput(const std::vector<std::string>& options, int currentSelection, bool& selectTriggered) {
    static bool keyUpPressed = false;
    static bool keyDownPressed = false;

    // Handle UP key with debounce
    if (graphics::getKeyState(graphics::SCANCODE_UP)) {
        if (!keyUpPressed) {
            currentSelection = (currentSelection - 1 + options.size()) % options.size();
            keyUpPressed = true;
        }
    }
    else {
        keyUpPressed = false;
    }

    // Handle DOWN key with debounce
    if (graphics::getKeyState(graphics::SCANCODE_DOWN)) {
        if (!keyDownPressed) {
            currentSelection = (currentSelection + 1) % options.size();
            keyDownPressed = true;
        }
    }
    else {
        keyDownPressed = false;
    }

    // Handle selection with ENTER
    if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {
        if (!selectTriggered) {
            selectTriggered = true;
        }
    }
    else {
        selectTriggered = false;
    }

    // Ensure currentSelection is clamped
    if (currentSelection < 0) currentSelection = 0;
    if (currentSelection >= static_cast<int>(options.size())) currentSelection = static_cast<int>(options.size()) - 1;

    return currentSelection;
}
