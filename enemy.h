#pragma once

#include "InteractiveObject.h"

class Enemy : public InteractiveObject {
private:
    bool isWeak; // Tracks whether enemy is weak

public:
    Enemy(GameState* state, int x, int y);
    void draw() override;
    void update(float dt) override;
    void init() override;
    void handleCollision(Player& player) override; // Now valid
    bool getIsWeak() const { return isWeak; }
    void setWeak(bool weak) { isWeak = weak; }
};
