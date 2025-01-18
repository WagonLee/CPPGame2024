#pragma once
#include "GridRenderer.h"
#include <vector>
#include <string>

class Menu {
protected:
    std::vector<std::vector<Tile>> menuGridState; // Grid for menu rendering
    int selectedOption = 0;                       // Current selection index

    void clearGrid(); // Clear the grid to black
    void drawTitle(const std::vector<std::string>& title, int row); // Draw a title
    void drawOptions(const std::vector<std::vector<std::string>>& options, int startRow); // Draw menu options

public:
    virtual ~Menu() = default;

    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
};
