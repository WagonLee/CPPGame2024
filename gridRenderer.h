#pragma once
#include <string>
#include <vector>
#include "config.h" // For grid dimensions

// Structure representing a single tile in the grid
struct Tile {
    float r, g, b;                 // Color components
    std::string texture = "";      // Texture path

    // Constructor for colors
    Tile(float red = 1.0f, float green = 1.0f, float blue = 1.0f)
        : r(red), g(green), b(blue), texture("") {}

    // Constructor for textures
    Tile(const std::string& texturePath)
        : r(1.0f), g(1.0f), b(1.0f), texture(texturePath) {}
};

// Declare the grid state as a 2D array of tiles
extern std::vector<std::vector<Tile>> gridState;

// Grid-related function declarations
void initGrid();  // Initialize the grid state
void drawGrid();  // Render the grid based on the current state
void updateGrid(); // Update the grid dynamically based on game events
