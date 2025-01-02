#include "Enemy.h"
#include "graphics.h"
#include "GameState.h"

// Constructor
Enemy::Enemy(GameState* state, int x, int y)
    : InteractiveObject(state, x, y, "Enemy"), isWeak(false) {}

// Draw the enemy
void Enemy::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    if (isWeak) {
        br.fill_color[0] = 0.5f; // Light red for weak state
        br.fill_color[1] = 0.0f;
        br.fill_color[2] = 0.0f;
    }
    else {
        br.fill_color[0] = 1.0f; // Strong red
        br.fill_color[1] = 0.0f;
        br.fill_color[2] = 0.0f;
    }

    float cellSize = 50.0f;
    float xPos = gridX * cellSize + cellSize / 2.0f;
    float yPos = gridY * cellSize + cellSize / 2.0f;

    graphics::drawRect(xPos, yPos, cellSize, cellSize, br);
}

// Update behavior
void Enemy::update(float dt) {}

// Initialize behavior
void Enemy::init() {}

// Handle collision
void Enemy::handleCollision(Player& player) {
    if (isWeak) {
        setActive(false); // Enemy disappears if weak
    }
    else {
        player.setDead(); // Player dies if enemy is strong
    }
}
