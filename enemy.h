#pragma once

#include "InteractiveObject.h"
#include <chrono> // For timing

class Enemy : public InteractiveObject {
private:
    bool isWeak; // Tracks whether enemy is weak

    // Smooth movement variables (Added to fix errors)
    float xPos, yPos;           // Exact position in pixels for smooth movement
    float targetX, targetY;     // Target positions for smooth movement
    bool moving;                // Tracks whether the enemy is currently moving
    float speed;                // Movement speed in grid squares per second

    // Movement timing
    std::chrono::time_point<std::chrono::high_resolution_clock> lastMoveTime; // Tracks last move
    static const int moveInterval = 2000; // Move every 2000ms (2 seconds)

    // Movement logic
    int directionX;
    int directionY;

    // Methods
    void randomizeDirection();   // Randomizes direction
    void moveToTarget(float dt); // Smooth movement to target (Fixed Declaration)

public:
    Enemy(GameState* state, int x, int y);
    void draw() override;
    void update(float dt) override;
    void init() override;
    void handleCollision(Player& player) override;

    bool getIsWeak() const { return isWeak; }
    void setWeak(bool weak) { isWeak = weak; }
};
