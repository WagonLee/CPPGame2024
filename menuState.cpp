#include "menustate.h"
#include "mainmenu.h"
#include <iostream>

MenuState* MenuState::instance = nullptr;

MenuState::MenuState() {
    currentMenu = MainMenu::getInstance(); // Start with Main Menu
}

MenuState* MenuState::getInstance() {
    if (!instance) {
        instance = new MenuState();
    }
    return instance;
}

void MenuState::switchMenu(Menu* newMenu) {
    currentMenu = newMenu;
    currentMenu->init();
}

void MenuState::startGame() {
    std::cout << "Starting the game..." << std::endl;
}

void MenuState::update(float dt) {
    if (currentMenu) {
        currentMenu->update(dt);
    }
}

void MenuState::draw() {
    if (currentMenu) {
        currentMenu->draw();
    }
}
