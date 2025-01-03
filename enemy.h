#pragma once

#include "InteractiveObject.h"
#include <chrono> // For timing

class Enemy : public InteractiveObject {
private:
    bool isWeak; // Tracks whether enemy is weak

    // Smooth movement variables
    float xPos, yPos;           // Exact position in pixels for smooth movement
    float targetX, targetY;     // Target positions for smooth movement
    bool moving;                // Tracks whether the enemy is currently moving
    float speed;                // Movement speed in grid squares per second

    // Movement timing
    std::chrono::time_point<std::chrono::high_resolution_clock> lastMoveTime; // Tracks last move
    int moveInterval;           // Tracks random move interval (1.5s–4s)

    // Movement logic
    int directionX;
    int directionY;

    // Methods
    void randomizeDirection();   // Randomizes direction
    void moveToTarget(float dt); // Smooth movement to target
    bool canMoveTo(int x, int y) const; // Checks if move is valid (NEW DECLARATION)

public:
    Enemy(GameState* state, int x, int y);
    void draw() override;
    void update(float dt) override;
    void init() override;
    void handleCollision(Player& player) override;

    bool getIsWeak() const { return isWeak; }
    void setWeak(bool weak) { isWeak = weak; }
};
