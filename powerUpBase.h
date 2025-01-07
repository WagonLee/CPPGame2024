#pragma once

#include "InteractiveObject.h"

class PowerUpBase : public InteractiveObject {
protected:
    int level; // Level 1, 2, or 3

public:
    PowerUpBase(GameState* state, int x, int y, int level);

    virtual void applyEffect() = 0; // Abstract method for effects

    // Fully implemented virtual methods
    virtual void update(float dt) override; // REQUIRED by GameObject
    virtual void draw() override;           // REQUIRED by GameObject
    virtual void init() override;           // REQUIRED by GameObject

    // Handle collision
    virtual void handleCollision(Player& player) override;

    // Getter for level
    int getLevel() const { return level; }

    // Virtual destructor
    virtual ~PowerUpBase() = default;
};
