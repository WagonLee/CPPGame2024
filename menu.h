#pragma once

#include "gridrenderer.h"
#include <vector>
#include <string>

class Menu {
protected:
    std::vector<std::vector<Tile>> menuGridState; 
    int selectedOption = 0;                        

    void clearGrid(); // Clear the grid to black
    void drawTitle(const std::vector<std::string>& title, int row); 
    void drawOptions(const std::vector<std::vector<std::string>>& options, int startRow); 

public:
    virtual ~Menu() = default;

    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
};
