#pragma once
#include "button.h"

class MainMenu {
public:
    MainMenu();
    void update();
    void draw();

private:
    Button startButton;
    Button optionsButton;
    Button exitButton;
};
