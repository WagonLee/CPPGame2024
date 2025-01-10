#include "graphics.h"
#include "GameState.h"
#include "Player.h"
#include "Menu.h"
#include "config.h"
#include <iostream> // For debug output
#include "GridRenderer.h"

bool inMenu = true;         // Default menu state

// Menu instance
Menu menu;

// Draw function
void draw() {
    if (inMenu) {
        menu.draw(); // Draw the menu
    }
    else {
        drawGrid();                       // Draw the alternating grid
        GameState::getInstance()->draw(); // Draw all game objects
    }
}

// Update function
void update(float dt) {
    if (inMenu) {
        menu.update(); // Update the menu
    }
    else {
        GameState::getInstance()->update(dt); // Update game objects
    }
}

// Initialize the game
void initGame() {
    GameState* gameState = GameState::getInstance();
    gameState->init();

    // Initialize player at the center of the playable grid
    int startX = GRID_WIDTH / 2; // Horizontal center
    int startY = UI_ROWS_ABOVE + (PLAYABLE_ROWS / 2); // Vertical center of the playable rows
    Player* player = new Player(gameState, startX, startY, 0.004f); // Slow speed
    gameState->addObject(player);

    std::cout << "Game initialized with player at (" << startX << ", " << startY << ")." << std::endl;
}

// Main function
int main() {
    // Create the window with default size
    graphics::createWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "DATAMINER");

    // Set logical canvas size and scaling mode
    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT); // Logical size for a square grid
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT); // Maintain aspect ratio with black bars

    // Initialize the menu
    menu.init();

    // Set callbacks for drawing and updating
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    // Start the game loop
    graphics::startMessageLoop();

    // Destroy the window on exit
    graphics::destroyWindow();

    return 0;
}