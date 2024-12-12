#include "player.h"
#include "graphics.h"
#include "config.h"
#include <cmath>

Player::Player() : x(CANVAS_WIDTH / 2), y(CANVAS_HEIGHT / 2), angle(0) {}

void Player::update(const std::vector<std::vector<int>>& map) {
    // Rotate left and right
    if (graphics::getKeyState(graphics::SCANCODE_A)) angle -= ROTATION_SPEED;
    if (graphics::getKeyState(graphics::SCANCODE_D)) angle += ROTATION_SPEED;

    // Move forward or backward
    float rad = angle * 3.14159f / 180.0f;
    float newX = x, newY = y;
    if (graphics::getKeyState(graphics::SCANCODE_W)) {
        newX += cos(rad) * PLAYER_SPEED;
        newY += sin(rad) * PLAYER_SPEED;
    }
    if (graphics::getKeyState(graphics::SCANCODE_S)) {
        newX -= cos(rad) * PLAYER_SPEED;
        newY -= sin(rad) * PLAYER_SPEED;
    }

    // Collision check
    int gridX = int(newX / CELL_SIZE);
    int gridY = int(newY / CELL_SIZE);
    if (map[gridY][gridX] == 0) { // Move only if the cell is empty
        x = newX;
        y = newY;
    }
}

void Player::draw() {
    graphics::Brush br;
    br.fill_color[0] = 0.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.0f;
    graphics::drawRect(x, y, 20, 20, br);

    // Optional: Draw a line showing the player's viewing angle
    graphics::drawLine(x, y, x + cos(angle * 3.14159f / 180.0f) * 30,
        y + sin(angle * 3.14159f / 180.0f) * 30, br);
}
