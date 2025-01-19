#include "collectible.h"
#include "graphics.h"
#include "gamestate.h"
#include "player.h" 
#include <iostream>  

Collectible::Collectible(GameState* state, int x, int y)
    : InteractiveObject(state, x, y, "Collectible") {}

void Collectible::draw() {
    graphics::Brush br;

    br.outline_opacity = 0.0f; 
    br.texture = ASSET_PATH + "objects/COLLECTIBLE.png"; 
    br.fill_opacity = 1.0f;   

    float xPos = gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float yPos = gridY * CELL_SIZE + CELL_SIZE / 2.0f;

    graphics::drawRect(xPos, yPos, CELL_SIZE, CELL_SIZE, br);
}

void Collectible::update(float dt) {
}


void Collectible::init() {
    std::cout << "Collectible initialized at: (" << gridX << ", " << gridY << ")\n";
}

void Collectible::handleCollision(Player& player) {
    std::cout << "Collectible collected at: (" << gridX << ", " << gridY << ")\n";

    graphics::playSound(ASSET_PATH + "sounds/collect.wav", 1.0f, false);

    player.addTailSegment(); 

    setActive(false); 
    GameState::getInstance()->scheduleCollectibleRespawn(); 
}
