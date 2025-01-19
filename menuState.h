#pragma once

#include "menu.h"

class MenuState {
private:
    static MenuState* instance;
    Menu* currentMenu = nullptr;

    MenuState();

public:
    static MenuState* getInstance();
    void switchMenu(Menu* newMenu);
    void startGame();

    void update(float dt);
    void draw();
};
