#include "Enemy.h"
#include "Player.h"

// Constructor
Enemy::Enemy(GameState* state, int x, int y, const std::string& type)
    : InteractiveObject(state, x, y, type), isWeak(false) {}

// Handle collision
void Enemy::handleCollision(Player& player) {
    if (isWeak) {
        setActive(false); // Disappear if weak
    }
    else {
        player.setDead(); // Kill the player if strong
    }
}

// Set weak state
void Enemy::setWeak(bool weak) {
    isWeak = weak;
}

// Get weak state
bool Enemy::getIsWeak() const {
    return isWeak;
}
