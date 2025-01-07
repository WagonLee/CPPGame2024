#pragma once

#include "InteractiveObject.h"

class GameState; // Forward declaration

class PowerUpBase : public InteractiveObject {
private:
    GameState* state; // Pointer to GameState for accessing timers and other data

protected:
    int level; // Level 1, 2, or 3

public:
    // Constructor
    PowerUpBase(GameState* state, int x, int y, int level);

    // Abstract method for applying effects
    virtual void applyEffect() = 0;

    // Required overrides from GameObject
    virtual void update(float dt) override; // Placeholder required by base class
    virtual void draw() override;           // Placeholder required by base class
    virtual void init() override;           // Placeholder required by base class

    // Handle collision behavior
    virtual void handleCollision(Player& player) override;

    // Getter for Power-Up level
    int getLevel() const { return level; }

    // Virtual destructor
    virtual ~PowerUpBase() = default;
};
