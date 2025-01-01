#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "graphics.h"

class Player : public GameObject {
private:
    int x, y;               // Grid position
    int directionX, directionY;     // Current movement direction
    int nextDirectionX, nextDirectionY; // Buffered direction changes
    float speed;            // Speed in grid squares per second
    float moveTimer;        // Timer to control movement intervals

public:
    Player(GameState* gs, int startX, int startY, float speed);
    void update(float dt) override;
    void draw() override;
    void init() override;

    void handleInput(); // Handles keyboard input for direction changes
};

#endif // PLAYER_H
