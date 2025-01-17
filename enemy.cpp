#include "enemy.h"
#include "player.h"
#include "gameState.h"

// Constructor
Enemy::Enemy(GameState* state, int x, int y, const std::string& type)
    : InteractiveObject(state, x, y, type), isWeak(false) {}

// Handle collision
void Enemy::handleCollision(Player& player) {
    // Ignore collisions if the enemy is inactive
    if (isInactive) {
        return;
    }

    if (isWeak) {
        std::cout << "Weak enemy killed by player!" << std::endl;
        setActive(false); // Deactivate the enemy

        auto* gameState = GameState::getInstance();

        // Notify GameState about the kill
        gameState->addToKillChain(); // Increment kill chain and calculate score
        gameState->enemyKilled = true;
        gameState->enemyKillEndTime = graphics::getGlobalTime() + 1500.0f; // 1.5 seconds from now
    }
    else {
        std::cout << "Player killed by enemy!" << std::endl;
        player.setDead(); // Kill the player if enemy is strong
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

// Set the enemy to inactive
void Enemy::setInactive(float duration) {
    isInactive = true;
    inactiveEndTime = graphics::getGlobalTime() + duration;
}

// Check if the enemy is inactive
bool Enemy::getIsInactive() const {
    return isInactive;
}

// Activate the enemy (end inactive state)
void Enemy::activate() {
    isInactive = false;
}