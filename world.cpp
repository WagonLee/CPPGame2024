#include "world.h"
#include "graphics.h"
#include "config.h"
#include <cmath>
#include <iostream>

World::World() {
    // Original map, with walls represented by 1 (red), 2 (green), 3 (yellow), and paths as 0
    map = {
        {1, 1, 1, 3, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 3, 0, 0, 0, 2, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 2, 2, 2, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 3, 1, 1, 3, 1, 1, 1, 1}
    };
}


void World::render3D(const Player& player) {
    graphics::Brush br;

    // Draw the ceiling (white)
    br.fill_color[0] = 1.0f;  // Red
    br.fill_color[1] = 1.0f;  // Green
    br.fill_color[2] = 1.0f;  // Blue
    br.fill_opacity = 1.0f;   // Full opacity
    br.outline_opacity = 0.0f;

    // The ceiling will be drawn from the top of the screen to half the canvas height
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 4, CANVAS_WIDTH, CANVAS_HEIGHT / 2, br);

    // Draw the floor (blue)
    br.fill_color[0] = 0.0f;  // Blue
    br.fill_color[1] = 0.0f;  // Green
    br.fill_color[2] = 1.0f;  // Blue
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT * 3 / 4, CANVAS_WIDTH, CANVAS_HEIGHT / 2, br);

    // Now, draw the walls with different colors depending on the wall type
    float rayAngle = player.angle - FOV / 2;
    for (int x = 0; x < CANVAS_WIDTH; x++) {
        float rad = rayAngle * 3.14159f / 180.0f;  // Convert ray angle to radians

        float distance = 0.0f;
        bool hitWall = false;
        float rayX = player.x;
        float rayY = player.y;

        // Cast the ray until it hits a wall or reaches max distance
        while (!hitWall && distance < 500.0f) {
            rayX += cos(rad);
            rayY += sin(rad);
            distance += 1.0f;

            int gridX = int(rayX / CELL_SIZE);
            int gridY = int(rayY / CELL_SIZE);

            if (gridX < 0 || gridY < 0 || gridX >= MAP_WIDTH || gridY >= MAP_HEIGHT || map[gridY][gridX] == 0) {
                hitWall = false;  // Continue casting ray if it's a path (0)
            }
            else {
                hitWall = true;  // Stop if it's a wall (1, 2, or 3)
            }
        }

        if (distance >= 500.0f) distance = 500.0f;  // Cap the distance to avoid rendering too far

        // Calculate the height of the wall based on distance
        float lineHeight = (CELL_SIZE * 500) / distance;

        // Determine the color based on the type of wall
        if (map[int(rayY / CELL_SIZE)][int(rayX / CELL_SIZE)] == 1) {
            br.fill_color[0] = 1.0f;  // Red for type 1 walls
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 0.0f;
        }
        else if (map[int(rayY / CELL_SIZE)][int(rayX / CELL_SIZE)] == 2) {
            br.fill_color[0] = 0.0f;  // Green for type 2 walls
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;
        }
        else if (map[int(rayY / CELL_SIZE)][int(rayX / CELL_SIZE)] == 3) {
            br.fill_color[0] = 1.0f;  // Yellow for type 3 walls
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;
        }

        // Draw the wall slice
        graphics::drawRect(x, CANVAS_HEIGHT / 2, 1, lineHeight, br);

        // Move to the next ray angle for the next slice
        rayAngle += FOV / CANVAS_WIDTH;
    }
}

