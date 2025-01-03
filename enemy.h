#pragma once

#include "InteractiveObject.h"

class Enemy : public InteractiveObject {
protected:
    bool isWeak; // Tracks whether enemy is weak

public:
    // Constructor
    Enemy(GameState* state, int x, int y, const std::string& type);

    // Common functionality
    virtual void handleCollision(Player& player) override;
    virtual void setWeak(bool weak); // Set weak state
    virtual bool getIsWeak() const; // Check weak state

    // Virtual destructor for polymorphic cleanup
    virtual ~Enemy() = default;
};
