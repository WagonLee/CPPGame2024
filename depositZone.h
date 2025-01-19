#pragma once

#include "gameobject.h"
#include "graphics.h"
#include <vector>

class DepositZone : public GameObject {
public:
    // Enum for zone shapes
    enum class Shape { STRAIGHT_LINE, DONUT, CIRCLE };

private:
    int gridX, gridY; // Start pos of zones              
    Shape shape;                   
    bool horizontal; // Straight line orientation               
    std::vector<std::pair<int, int>> tiles; 
    std::vector<int> tileVariants; // For randomizing horiz line
    float duration;                
    float timer; // TTL timers                   

    void drawStraightLine();
    void drawDonut();
    void drawCircle();

    // Helper method to initialize random variants
    void initializeVariants();

public:
    DepositZone(GameState* state, int x, int y, Shape shape, bool horizontal = true);

    // Override methods
    void update(float dt) override;   
    void draw() override;              
    void init() override;             

    // Utility methods
    bool isTileInZone(int x, int y) const; 
    bool isExpired() const { return timer <= 0.0f; } 
    void resetTimer() { timer = duration; }      

    // Accessor for tiles
    const std::vector<std::pair<int, int>>& getTiles() const { return tiles; } 

    // Shape calculation helpers
    void createStraightLine();
    void createDonut();
    void createCircle();
};
