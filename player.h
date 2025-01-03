#pragma once

#include "GameObject.h"
#include "graphics.h"
#include <vector> // For tail storage

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

    // Tail properties
    struct TailSegment {
        int x, y;
    };
    std::vector<TailSegment> tail; // Tail storage

public:
    // Constructor
    Player(GameState* gs, int startX, int startY, float speed);

    // Core methods
    void update(float dt) override;   // Updates player state each frame
    void draw() override;             // Draws the player and tail
    void init() override;             // Initializes the player state

    // Input handling
    void handleInput();               // Handles keyboard input for direction changes

    // Movement
    void moveToTarget(float dt);      // Smooth movement to target position

    // Collision handling
    void checkCollision();            // Checks collision with the edge

    // Tail-related methods
    void addTailSegment();            // Adds a tail segment to the player

    // Position accessors
    int getGridX() const { return gridX; }  // Get X position in grid
    int getGridY() const { return gridY; }  // Get Y position in grid

    // Debugging
    int getTailSize() const { return tail.size(); } // Returns the tail size for debugging

    // Death handling
    void setDead();                    // Sets player as dead and stops activity
    bool getIsAlive() const;           // Checks if player is alive
};
