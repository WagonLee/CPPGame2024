#pragma once
#include "player.h"
#include <vector>

class World {
public:
    std::vector<std::vector<int>> map;

    World();
    void render3D(const Player& player);
};
