#pragma once
#include <string>
#include <vector>
#include "config.h" // For grid dimensions

// Structure representing a single tile in the grid
struct Tile {
    float r, g, b;       // RGB color of the tile
    std::string texture; // Path to texture (optional)
    bool needsUpdate;    // Whether the tile needs redrawing

    // Constructor
    Tile(float red = 1.0f, float green = 1.0f, float blue = 1.0f, const std::string& tex = "")
        : r(red), g(green), b(blue), texture(tex), needsUpdate(false) {}
};

// Declare the grid state as a 2D array of tiles
extern std::vector<std::vector<Tile>> gridState;

// Grid-related function declarations
void initGrid();  // Initialize the grid state
void drawGrid();  // Render the grid based on the current state
void updateGrid(); // Update the grid dynamically based on game events
