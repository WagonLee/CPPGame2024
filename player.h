#pragma once

#include "GameObject.h"
#include "graphics.h"

class Player : public GameObject {
private:
    float x, y;               // Exact position in pixels for smooth movement
    int gridX, gridY;         // Current grid position
    int directionX, directionY;     // Current movement direction
    int nextDirectionX, nextDirectionY; // Buffered direction changes
    float speed;            // Speed in grid squares per second
    float targetX, targetY; // Target position for smooth transitions
    bool moving;            // True if moving towards a target
    bool isAlive;           // Tracks whether the player is alive
    bool hitEdge;           // Placeholder to check if edge collision occurred

public:
    Player(GameState* gs, int startX, int startY, float speed);
    void update(float dt) override;
    void draw() override;
    void init() override;

    void handleInput(); // Handles keyboard input for direction changes
    void moveToTarget(float dt); // Smooth movement to target position
    void checkCollision(); // Checks collision with the edge
};
