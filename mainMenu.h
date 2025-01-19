#pragma once

#include "menu.h"

class MainMenu : public Menu {
private:
    static MainMenu* instance;
    int debounceFrameCount = 0;
    const int DEBOUNCE_FRAMES = 10;

    MainMenu(); 

public:
    static MainMenu* getInstance();

    void init() override;
    void update(float dt) override;
    void draw() override;
    float currCooldown;
};
