#pragma once

#include "InteractiveObject.h"

class Collectible : public InteractiveObject {
private:
    bool isLingering = false;   // Flag for lingering state
    float lingeringEndTime = 0; // Time when the collectible should disappear

public:
    Collectible(GameState* state, int x, int y);
    void draw() override;
    void update(float dt) override;
    void init() override;
    void handleCollision(Player& player) override; // No changes needed

    void startLingering(float duration); // Start lingering for a duration
    bool getIsLingering() const;         // Check if the collectible is lingering
};
