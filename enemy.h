#pragma once

#include "InteractiveObject.h"

class Enemy : public InteractiveObject {
protected:
    bool isWeak; // Tracks whether enemy is weak
    float inactiveEndTime = 0.0f;    // Time when the inactive state ends

public:
    // Constructor
    Enemy(GameState* state, int x, int y, const std::string& type);

    // Common functionality
    virtual void handleCollision(Player& player) override;
    virtual void setWeak(bool weak); // Set weak state
    virtual bool getIsWeak() const; // Check weak state
    bool isInactive = false;         // Tracks whether the enemy is inactive
    void setInactive(float duration);
    bool getIsInactive() const;
    void activate();

    // Virtual destructor for polymorphic cleanup
    virtual ~Enemy() = default;
};
