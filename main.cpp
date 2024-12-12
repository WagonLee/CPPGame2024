#include "graphics.h"
#include "player.h"
#include "world.h"
#include "config.h"

Player player;
World world;

void update(float ms) {
    player.update(world.map);  // Player update with map collision logic
}

void draw() {
    world.render3D(player);  // Render the pseudo-3D world
}

int main() {
    graphics::createWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "Battlezone Demo");

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);
    graphics::startMessageLoop();  // Start the main game loop

    return 0;
}
