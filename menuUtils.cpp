#include "MenuUtils.h"
#include "graphics.h"

// This function reads UP, DOWN, and RETURN keys using a static memory
// of previous states. It returns the new selectedOption. If user hits
// RETURN, we set selectTriggered = true exactly once.

int handleMenuInput(const std::vector<std::string>& options,
    int currentOption,
    bool& selectTriggered)
{
    // Store the old states across function calls
    static bool prevUp = false;
    static bool prevDown = false;
    static bool prevReturn = false;

    // Read current states
    bool currUp = graphics::getKeyState(graphics::SCANCODE_UP);
    bool currDown = graphics::getKeyState(graphics::SCANCODE_DOWN);
    bool currReturn = graphics::getKeyState(graphics::SCANCODE_RETURN);

    // Reset the output each frame
    selectTriggered = false;

    // ========================
    // Up Key - rising edge
    // ========================
    if (currUp && !prevUp) {
        // Move selection up
        currentOption--;
    }

    // ========================
    // Down Key - rising edge
    // ========================
    if (currDown && !prevDown) {
        // Move selection down
        currentOption++;
    }

    // ========================
    // Wrap/clamp selection
    // ========================
    if (!options.empty()) {
        // For wrapping behavior:
        currentOption = (currentOption + (int)options.size()) % (int)options.size();
    }
    else {
        // If somehow no options, clamp to 0
        currentOption = 0;
    }

    // ========================
    // Enter Key - rising edge
    // ========================
    if (currReturn && !prevReturn) {
        // The user pressed enter this frame
        selectTriggered = true;
    }

    // Update static "prev" states
    prevUp = currUp;
    prevDown = currDown;
    prevReturn = currReturn;

    // Return the new selection
    return currentOption;
}
