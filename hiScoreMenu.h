#pragma once
#include "Menu.h"
#include <vector>

class HiScoreMenu : public Menu {
private:
    static HiScoreMenu* instance;

    // Debounce for leftover key presses
    int debounceFrameCount = 0;
    const int DEBOUNCE_FRAMES = 10;

    std::vector<int> scores;

    HiScoreMenu();

public:
    static HiScoreMenu* getInstance();

    void init() override;
    void update(float dt) override;
    void draw() override;
};
