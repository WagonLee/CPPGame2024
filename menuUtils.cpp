#include "menuutils.h"
#include "graphics.h"

int handleMenuInput(const std::vector<std::string>& options,
    int currentOption,
    bool& selectTriggered)
{
    static bool prevUp = false;
    static bool prevDown = false;
    static bool prevReturn = false;

    bool currUp = graphics::getKeyState(graphics::SCANCODE_UP);
    bool currDown = graphics::getKeyState(graphics::SCANCODE_DOWN);
    bool currReturn = graphics::getKeyState(graphics::SCANCODE_RETURN);

    selectTriggered = false;


    if (currUp && !prevUp) {
        currentOption--;
    }

    if (currDown && !prevDown) {
        currentOption++;
    }

    if (!options.empty()) {
        currentOption = (currentOption + (int)options.size()) % (int)options.size();
    }
    else {
        currentOption = 0;
    }

    if (currReturn && !prevReturn) {
        selectTriggered = true;
    }

    prevUp = currUp;
    prevDown = currDown;
    prevReturn = currReturn;

    return currentOption;
}
