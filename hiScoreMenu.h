#pragma once
#include "Menu.h"
#include <vector>


class HiScoreMenu : public Menu {
private:
    static HiScoreMenu* instance;

    // Debounce for leftover key presses
    int debounceFrameCount = 0;
    const int DEBOUNCE_FRAMES = 10;

    static std::vector<int> scores;
    static bool defaultScoresInitialized; // Ensures default scores are only set once

    HiScoreMenu();

public:
    static HiScoreMenu* getInstance();

    void init() override;
    void update(float dt) override;
    void draw() override;

    void updateLeaderboard(int score); // Update leaderboard with a new score
    void initDefaultScores(); // Initialize default scores if needed
};
