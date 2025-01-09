#pragma once

#include "Enemy.h"
#include <chrono> // For timing

class MovingEnemy : public Enemy {
private:
    // Smooth movement variables
    float xPos, yPos;           // Exact position in pixels
    float targetX, targetY;     // Target positions for smooth movement
    bool moving;                // Tracks if currently moving
    float speed;                // Movement speed

    // Timing variables
    std::chrono::time_point<std::chrono::high_resolution_clock> lastMoveTime;
    int moveInterval;           // Random interval (1.5–4s)

    // Movement logic
    int directionX, directionY;

    // Methods
    void randomizeDirection();   // Randomizes direction
    void moveToTarget(float dt); // Smooth movement logic
    bool canMoveTo(int x, int y) const; // Checks if the move is valid

    bool isWeakState = false; // Tracks if the enemy is weak
    bool stopped = false;     // Tracks if movement is stopped while weak

public:
    // Constructor with weak state initialization
    MovingEnemy(GameState* state, int x, int y, bool weak = false);

    void draw() override;
    void update(float dt) override;
    void init() override;

    // Set and check weak state
    void setWeak(bool weak);
    bool isWeak() const { return isWeakState; }

    // Control movement when weak
    void stopMovement();
    void startMovement();

    // Collision handling (override base)
    void handleCollision(Player& player) override;
};
