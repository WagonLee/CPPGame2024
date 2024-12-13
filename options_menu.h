#pragma once
#include "button.h"

class OptionsMenu {
public:
    OptionsMenu();
    void update();
    void draw();

private:
    Button backButton;
};
