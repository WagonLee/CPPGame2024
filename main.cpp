#include "graphics.h"
#include "GameState.h"
#include "Player.h"

const int GRID_SIZE = 12;
const float CELL_SIZE = 50.0f; // 50 pixels per cell

void drawGrid() {
    graphics::Brush brush;
    brush.fill_opacity = 1.0f;
    brush.fill_color[0] = 0.8f; // Light gray
    brush.fill_color[1] = 0.8f;
    brush.fill_color[2] = 0.8f;

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            float x = col * CELL_SIZE + CELL_SIZE / 2;
            float y = row * CELL_SIZE + CELL_SIZE / 2;
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, brush);
        }
    }
}

void draw() {
    drawGrid(); // Draw the grid as individual squares
    GameState::getInstance()->draw(); // Draw all game objects
}

void update(float dt) {
    GameState::getInstance()->update(dt); // Update game objects
}

void init() {
    GameState* gameState = GameState::getInstance();
    gameState->init();

    // Initialize player at the center of the grid
    int startX = GRID_SIZE / 2;
    int startY = GRID_SIZE / 2;
    Player* player = new Player(gameState, startX, startY, 0.002f); // Slow down the speed
    gameState->addObject(player);
}

int main() {
    graphics::createWindow(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE, "Snake Game Prototype");
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);
    init(); // Initialize game objects
    graphics::startMessageLoop();
    graphics::destroyWindow();
    return 0;
}
