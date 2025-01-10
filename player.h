#pragma once

#include "GameObject.h"
#include "graphics.h"
#include <vector> // For tail storage

class Player : public GameObject {
private:
    // Positioning
    float x, y;                // Exact position in pixels for smooth movement
    int gridX, gridY;          // Current grid position
    float targetX, targetY;    // Target position for smooth transitions

    // Movement
    int directionX, directionY;        // Current movement direction
    int nextDirectionX, nextDirectionY; // Buffered direction changes
    float speed;                       // Speed in grid squares per second
    bool moving;                       // True if moving towards a target

    // State
    bool isAlive;            // Tracks whether the player is alive
    bool hitEdge;            // Tracks edge collision for special states

    // Tail properties
    struct TailSegment {
        int gridX, gridY;      // Grid position
        float x, y;            // Smooth pixel position
        float targetX, targetY; // Target positions for smooth movement
    };
    std::vector<TailSegment> tail;    // Tail storage

    // Grant GameState access to private members
    friend class GameState;

public:
    // Constructor
    Player(GameState* gs, int startX, int startY, float speed);

    // Core methods
    void update(float dt) override;    // Updates player state each frame
    void draw() override;              // Draws the player and tail
    void init() override;              // Initializes or resets the player state

    // Input handling
    void handleInput();                // Handles keyboard input for direction changes

    // Movement
    void moveToTarget(float dt);       // Smooth movement to target position
    void updateTail(float dt);         // Smooth tail movement update

    // Collision handling
    void checkCollision();             // Checks collision with the grid edges
    void checkTailCollision();         // Checks collision with the tail

    // Tail-related methods
    void addTailSegment();             // Adds a new segment to the tail
    void shedTail();                   // Sheds the tail and spawns enemies

    // Position accessors
    int getGridX() const { return gridX; }  // Get X position in the grid
    int getGridY() const { return gridY; }  // Get Y position in the grid

    // Debugging
    int getTailSize() const { return tail.size(); } // Returns the tail size for debugging

    // Death handling
    void setDead();                    // Sets the player as dead and stops activity
    bool getIsAlive() const;           // Checks if the player is alive
};
