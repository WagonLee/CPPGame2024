#include "stationaryenemy.h"
#include "gamestate.h"
#include <iostream>
#include "config.h"

StationaryEnemy::StationaryEnemy(GameState* state, int x, int y)
    : Enemy(state, x, y, "StationaryEnemy") {
    setInactive(4.0f); 
}

void StationaryEnemy::init() {
}

void StationaryEnemy::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.0f; 

    if (isInactive) {
        br.texture = ASSET_PATH + "objects/STATIONARY-ENEMY-SPAWNING.png";
    }
    else if (isWeak) {
        br.texture = ASSET_PATH + "objects/STATIONARY-ENEMY-WEAK.png"; 
    }
    else {
        br.texture = ASSET_PATH + "objects/STATIONARY-ENEMY.png"; 
    }

    br.fill_opacity = 1.0f; 

    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
}

void StationaryEnemy::update(float dt) {
    if (isInactive) {
        if (graphics::getGlobalTime() >= inactiveEndTime) {
            activate(); 
            std::cout << "StationaryEnemy at (" << gridX << ", " << gridY << ") is now ACTIVE." << std::endl;
        }
        return; 
    }
}
