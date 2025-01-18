#pragma once
#include "Menu.h"
#include "MenuUtils.h"

class MainMenu : public Menu {
private:
    static MainMenu* instance;
    float cooldown = 0.2f;
    float currCooldown = 0.0f;

    MainMenu();

public:
    static MainMenu* getInstance();

    void init() override;
    void update(float dt) override;
    void draw() override;
};
