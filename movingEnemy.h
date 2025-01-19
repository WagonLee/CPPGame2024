#pragma once

#include "enemy.h"
#include <chrono> // non graphics clock logic...

class MovingEnemy : public Enemy {
private:
    // Smooth movement variables
    float xPos, yPos;           
    float targetX, targetY;     
    bool moving;                
    float speed;                

    // Timing variables NON GRAPHICS CLOCK... MY BAD DIDN'T KNOW LOL
    std::chrono::time_point<std::chrono::high_resolution_clock> lastMoveTime;
    int moveInterval;         

    // Movement logic
    int directionX, directionY;

    void randomizeDirection();   
    void moveToTarget(float dt); 
    bool canMoveTo(int x, int y) const; 

    bool isWeakState = false; 
    bool stopped = false;     

public:
    MovingEnemy(GameState* state, int x, int y, bool weak = false);

    void draw() override;
    void update(float dt) override;
    void init() override;

    void setWeak(bool weak);
    bool isWeak() const { return isWeakState; }

    void stopMovement();
    void startMovement();

    void handleCollision(Player& player) override;
};
