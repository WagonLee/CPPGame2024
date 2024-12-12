#include "Enemy.h"
#include "graphics.h"
#include "config.h"
#include <cmath>

Enemy::Enemy(float cx, float cy) : centerX(cx), centerY(cy), angle(0) {}

void Enemy::update() {
    angle += 1; // Circular motion
}

void Enemy::draw() {
    float x = centerX + cos(angle * 3.14159f / 180.0f) * ENEMY_RADIUS;
    float y = centerY + sin(angle * 3.14159f / 180.0f) * ENEMY_RADIUS;

    graphics::Brush br;
    br.texture = "assets/circle.png"; // Placeholder for circular PNG
    graphics::drawRect(x, y, 20, 20, br); // Simplified enemy as a square with texture
}
