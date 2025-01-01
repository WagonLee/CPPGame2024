#include "graphics.h"
#include "GameState.h"
#include "Player.h"

const int GRID_SIZE = 12;
const float CELL_SIZE = 50.0f; // 50 pixels per cell

void drawGrid() {
    graphics::Brush brush;

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            // Alternate colors
            if ((row + col) % 2 == 0) {
                brush.fill_color[0] = 0.0f; // Black
                brush.fill_color[1] = 0.0f;
                brush.fill_color[2] = 0.0f;
            }
            else {
                brush.fill_color[0] = 1.0f; // White
                brush.fill_color[1] = 1.0f;
                brush.fill_color[2] = 1.0f;
            }

            brush.fill_opacity = 1.0f;

            float x = col * CELL_SIZE + CELL_SIZE / 2;
            float y = row * CELL_SIZE + CELL_SIZE / 2;
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, brush);
        }
    }
}

void draw() {
    drawGrid(); // Draw the alternating grid
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
    Player* player = new Player(gameState, startX, startY, 0.002f); // Slow speed
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
