#pragma once
#include <vector>

class Player {
public:
    float x, y, angle;

    Player();
    void update(const std::vector<std::vector<int>>& map);
    void draw(); // Optional: Draw player in 2D map
};
