#include "graphics.h"
#include "GameState.h"
#include "Player.h"
#include "Menu.h" // Include the new Menu class

const int GRID_SIZE = 12;
const float CELL_SIZE = 50.0f; // 50 pixels per cell

// Global flag for toggling between the menu and gameplay
bool inMenu = true;

// Menu instance
Menu menu;

// Function to draw the game grid
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

// Updated draw function
void draw() {
    if (inMenu) {
        menu.draw(); // Draw the menu
    }
    else {
        drawGrid();                       // Draw the alternating grid
        GameState::getInstance()->draw(); // Draw all game objects
    }
}

// Updated update function
void update(float dt) {
    if (inMenu) {
        menu.update(); // Update the menu
    }
    else {
        GameState::getInstance()->update(dt); // Update game objects
    }
}

// Initialize game objects
void initGame() {
    GameState* gameState = GameState::getInstance();
    gameState->init();

    // Initialize player at the center of the grid
    int startX = GRID_SIZE / 2;
    int startY = GRID_SIZE / 2;
    Player* player = new Player(gameState, startX, startY, 0.004f); // Slow speed
    gameState->addObject(player);
}

// Updated main function
int main() {
    graphics::createWindow(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE, "Snake Game");

    // Initialize the menu
    menu.init();

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    // Add a key listener for switching from menu to gameplay
    if (inMenu && graphics::getKeyState(graphics::SCANCODE_RETURN)) {
        inMenu = false;   // Switch to gameplay
        initGame();       // Initialize the game when starting from the menu
    }

    graphics::startMessageLoop();
    graphics::destroyWindow();
    return 0;
}
