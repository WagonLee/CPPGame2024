#pragma once
#include "Menu.h"
#include "GridRenderer.h"
#include <vector>
#include <string>


class HiScoreMenu : public Menu {
private:
    static HiScoreMenu* instance;

    std::vector<int> scores;                // Top 10 scores
    std::vector<std::vector<Tile>> grid;    // Grid for hiScore rendering
    int selectedOption = 0;                 // Tracks "Back" option selection

    HiScoreMenu();

    float inputCooldown;
    float lastInputTime;

public:
    static HiScoreMenu* getInstance();

    void init() override;
    void update(float dt) override;
    void draw() override;
};
