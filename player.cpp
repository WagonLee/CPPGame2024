#include "Player.h"

Player::Player(GameState* gs, int startX, int startY, float speed)
    : GameObject(gs, "Player"), x(startX), y(startY), directionX(0), directionY(-1), speed(speed), moveTimer(0.0f),
    nextDirectionX(0), nextDirectionY(-1) {}

void Player::init() {
    directionX = 0;
    directionY = -1; // Start moving up
    nextDirectionX = 0;
    nextDirectionY = -1;
    moveTimer = 0.0f;
}

void Player::update(float dt) {
    // Update movement timer
    moveTimer += dt;

    // Handle input for direction changes
    handleInput();

    // Move at fixed intervals
    if (moveTimer >= 1.0f / speed) {
        // Apply buffered direction change at the move step
        directionX = nextDirectionX;
        directionY = nextDirectionY;

        // Update position based on direction
        x += directionX;
        y += directionY;

        // Keep player within bounds
        if (x < 0) x = 0;
        if (x >= 12) x = 11;
        if (y < 0) y = 0;
        if (y >= 12) y = 11;

        moveTimer = 0.0f; // Reset timer
    }
}

void Player::draw() {
    graphics::Brush brush;
    brush.fill_color[0] = 0.0f; // Green
    brush.fill_color[1] = 1.0f;
    brush.fill_color[2] = 0.0f;
    brush.fill_opacity = 1.0f;

    float centerX = x * 50.0f + 25.0f;
    float centerY = y * 50.0f + 25.0f;
    graphics::drawRect(centerX, centerY, 50.0f, 50.0f, brush);
}

void Player::handleInput() {
    // Buffer direction changes, apply them only on the next move step
    if (graphics::getKeyState(graphics::SCANCODE_UP) && directionY == 0) {
        nextDirectionX = 0;
        nextDirectionY = -1;
    }
    if (graphics::getKeyState(graphics::SCANCODE_DOWN) && directionY == 0) {
        nextDirectionX = 0;
        nextDirectionY = 1;
    }
    if (graphics::getKeyState(graphics::SCANCODE_LEFT) && directionX == 0) {
        nextDirectionX = -1;
        nextDirectionY = 0;
    }
    if (graphics::getKeyState(graphics::SCANCODE_RIGHT) && directionX == 0) {
        nextDirectionX = 1;
        nextDirectionY = 0;
    }
}
