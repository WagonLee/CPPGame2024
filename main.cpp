#include "graphics.h"
#include "GameState.h"
#include "Player.h"
#include "Menu.h"
#include "config.h"
#include <iostream> // For debug output

bool inMenu = true;         // Default menu state

// Menu instance
Menu menu;

// Function to draw the game grid
void drawGrid() {
    graphics::Brush brush;

    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            // Determine color based on row
            if (row < UI_ROWS_ABOVE) {
                // UI rows above (blue)
                brush.fill_color[0] = 0.0f; // Blue
                brush.fill_color[1] = 0.0f;
                brush.fill_color[2] = 1.0f;
            }
            else if (row >= UI_ROWS_ABOVE + PLAYABLE_ROWS) {
                // UI rows below (blue)
                brush.fill_color[0] = 0.0f; // Blue
                brush.fill_color[1] = 0.0f;
                brush.fill_color[2] = 1.0f;
            }
            else {
                // Playable rows (alternating black and white)
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
            }

            // Draw the cell
            brush.fill_opacity = 1.0f;
            float x = col * CELL_SIZE + CELL_SIZE / 2;
            float y = row * CELL_SIZE + CELL_SIZE / 2;
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, brush);
        }
    }
}

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

    // Initialize the game
    initGame();

    // Start the game loop
    graphics::startMessageLoop();

    // Destroy the window on exit
    graphics::destroyWindow();

    return 0;
}
