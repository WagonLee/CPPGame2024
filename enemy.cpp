#include "Enemy.h"
#include "graphics.h"

// Constructor
Enemy::Enemy(GameState* state, int x, int y)
    : InteractiveObject(state, x, y, "Enemy") {}

// Draw the enemy (red square)
void Enemy::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.fill_color[0] = 1.0f;
    br.fill_color[1] = 0.0f;
    br.fill_color[2] = 0.0f;

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}

// Update logic for enemy
void Enemy::update(float dt) {
    // Placeholder for future movement or behavior
}

// Initialize enemy
void Enemy::init() {
    // Placeholder for initialization logic
}

// Handle collision
void Enemy::handleCollision() {
    // Placeholder for collision behavior
}
