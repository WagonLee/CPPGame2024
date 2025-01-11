#pragma once

#include "GameObject.h"
#include "graphics.h"
#include <vector>

class DepositZone : public GameObject {
public:
    // Enum for zone shapes
    enum class Shape { STRAIGHT_LINE, DONUT, CIRCLE };

private:
    int gridX, gridY;              // Starting position
    Shape shape;                   // Shape of the deposit zone
    bool horizontal;               // Orientation for straight lines
    std::vector<std::pair<int, int>> tiles; // Stores affected grid tiles
    float duration;                // Active duration (seconds)
    float timer;                   // Tracks remaining time

    void drawStraightLine();
    void drawDonut();
    void drawCircle();

public:
    // Constructor
    DepositZone(GameState* state, int x, int y, Shape shape, bool horizontal = true);

    // Override methods
    void update(float dt) override;   // Update timer
    void draw() override;             // Draw zone
    void init() override;             // Initialize tiles based on shape

    // Utility methods
    bool isTileInZone(int x, int y) const; // Check if a tile is in the zone
    bool isExpired() const { return timer <= 0.0f; } // Check if expired
    void resetTimer() { timer = duration; }      // Reset zone timer

    // Accessor for tiles
    const std::vector<std::pair<int, int>>& getTiles() const { return tiles; } // Read-only access

    // Shape calculation helpers
    void createStraightLine();
    void createDonut();
    void createCircle();
};
