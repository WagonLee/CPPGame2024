#include "world.h"
#include "graphics.h"
#include "config.h"
#include <cmath>
#include <iostream>

//please work

World::World() {
    // Static map with walls (1) and open spaces (0)
    map = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };
}

#include <cmath>  // for cos(), sin()

void World::render3D(const Player& player) {
    graphics::Brush br;
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

            if (gridX < 0 || gridY < 0 || gridX >= MAP_WIDTH || gridY >= MAP_HEIGHT || map[gridY][gridX] == 1) {
                hitWall = true;
            }
        }

        if (distance >= 500.0f) distance = 500.0f;  // Cap the distance to avoid rendering too far

        // Calculate the height of the wall based on distance
        float lineHeight = (CELL_SIZE * 500) / distance;  // Scale wall height based on ray distance

        // Set color (pure red) for the wall
        br.fill_color[0] = 1.0f;  // Red
        br.fill_color[1] = 0.0f;  // No green
        br.fill_color[2] = 0.0f;  // No blue
        br.fill_opacity = 1.0f;   // Full opacity

        // Remove the outline
        br.outline_opacity = 0.0f;

        // Draw the wall slice
        graphics::drawRect(x, CANVAS_HEIGHT / 2, 1, lineHeight, br);  // Vertical line representing the wall

        // Move to the next ray angle for the next slice
        rayAngle += FOV / CANVAS_WIDTH;
    }
}
