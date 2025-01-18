#pragma once
#include "Menu.h"

class MainMenu : public Menu {
private:
    static MainMenu* instance;

    // Existing variables:
    // int selectedOption;
    // float currCooldown;

    // ADD: For a short input debounce after opening the menu
    int debounceFrameCount = 0;
    const int DEBOUNCE_FRAMES = 10;

    MainMenu(); // Private constructor

public:
    static MainMenu* getInstance();

    void init() override;
    void update(float dt) override;
    void draw() override;
    float currCooldown;
};
