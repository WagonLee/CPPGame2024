#pragma once

#include "gameobject.h"
#include "graphics.h"
#include <vector> 

class Player : public GameObject {
private:
    float x, y;              
    int gridX, gridY;       
    int directionX, directionY;     
    int nextDirectionX, nextDirectionY; 
    float speed;            
    float targetX, targetY; 
    bool moving;          
    bool isAlive;       
    bool hitEdge;         

    // Tail properties
    struct TailSegment {
        int gridX, gridY;    
        float x, y;       
        float targetX, targetY; 
    };
    std::vector<TailSegment> tail; 

    // Grant GameState access 
    friend class GameState;

public:
    Player(GameState* gs, int startX, int startY, float speed);

    void update(float dt) override;   
    void draw() override;              
    void init() override;             

    void handleInput();               

    void moveToTarget(float dt);      
    void updateTail(float dt);        

    void checkCollision();            
    void checkTailCollision();        

    void addTailSegment();            
    void shedTail();                  

    int getGridX() const { return gridX; }  
    int getGridY() const { return gridY; }  

    int getTailSize() const { return tail.size(); } // debug

    void setDead();                    
    bool getIsAlive() const;           
};