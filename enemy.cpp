#include "enemy.h"
#include "player.h"
#include "gamestate.h"

Enemy::Enemy(GameState* state, int x, int y, const std::string& type)
    : InteractiveObject(state, x, y, type), isWeak(false) {}

void Enemy::handleCollision(Player& player) {
    // Ignore collisions if the enemy is inactive
    if (isInactive) {
        return;
    }

    if (isWeak) {
        graphics::playSound(ASSET_PATH + "sounds/kill.wav", 0.7f, false);
        std::cout << "Weak enemy killed by player!" << std::endl;
        setActive(false); 

        auto* gameState = GameState::getInstance();

        // Notify GameState about the kill
        gameState->addToKillChain(); // Increment kill chain and calculate score
        gameState->enemyKilled = true;
        gameState->enemyKillEndTime = graphics::getGlobalTime() + 1500.0f; 
    }
    else {
        std::cout << "Player killed by enemy!" << std::endl;
        player.setDead();
    }
}


void Enemy::setWeak(bool weak) {
    isWeak = weak;
}

bool Enemy::getIsWeak() const {
    return isWeak;
}

void Enemy::setInactive(float duration) {
    isInactive = true;
    inactiveEndTime = graphics::getGlobalTime() + duration;
}

bool Enemy::getIsInactive() const {
    return isInactive;
}

void Enemy::activate() {
    isInactive = false;
}