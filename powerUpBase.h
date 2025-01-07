#pragma once

#include "InteractiveObject.h"

class GameState;        // Forward declare GameState

class PowerUpBase : public InteractiveObject {
protected:
    GameState* state; // Pointer to GameState for accessing timers and data

    int level; // Level of the Power-Up (1, 2, or 3)

    // Shared Weakness Effect Variables
    float weakEffectDuration = 0.0f; // Duration for the weakness effect
    bool isWeakEffectActive = false; // Tracks if the effect is active

public:
    // Constructor
    PowerUpBase(GameState* state, int x, int y, int level);

    // Apply Effect (abstract, overridden in derived classes)
    virtual void applyEffect() = 0;

    // Overrides from GameObject
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void init() override;

    // Weakness Effect Methods (shared across levels)
    void startWeakEffect(float duration);  // Start the timer for weak effect
    void updateWeakEffect(float dt);       // Handle timing updates
    void endWeakEffect();                  // Reset enemies after effect

    // Getter for Power-Up level
    int getLevel() const { return level; }

    // Collision handling
    virtual void handleCollision(Player& player) override;

    // Virtual destructor
    virtual ~PowerUpBase() = default;
};
