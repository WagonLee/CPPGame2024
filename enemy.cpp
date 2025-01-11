#include "enemy.h"
#include "player.h"
#include "gameState.h"

// Constructor
Enemy::Enemy(GameState* state, int x, int y, const std::string& type)
    : InteractiveObject(state, x, y, type), isWeak(false) {}

// Handle collision
void Enemy::handleCollision(Player& player) {
    if (isWeak) {
        std::cout << "Weak enemy killed by player!" << std::endl;
        setActive(false); // Deactivate the enemy

        auto* gameState = GameState::getInstance();

        // Notify GameState about the kill
        gameState->addToKillChain(); // Increment kill chain and calculate score
        gameState->enemyKilled = true;
        gameState->enemyKillEndTime = graphics::getGlobalTime() + 1000.0f; // 1 second from now
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
