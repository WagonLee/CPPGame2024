#pragma once
#include "Menu.h"

class TutorialMenu : public Menu {
private:
    static TutorialMenu* instance;

    TutorialMenu();

public:
    static TutorialMenu* getInstance();

    void init() override;
    void update(float dt) override;
    void draw() override;
};
