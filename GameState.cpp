#include "GameState.h"
#include "DepositZone.h"
#include <iostream>  // Debug output added
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "MovingEnemy.h"
#include "StationaryEnemy.h"
#include "Collectible.h"
#include "PowerUpBlue.h"
#include "Config.h"


GameState* GameState::instance = nullptr;

// Constructor
GameState::GameState() {}

// Singleton instance
GameState* GameState::getInstance() {
    if (!instance) {
        instance = new GameState();
    }
    return instance;
}

// Add object to game
void GameState::addObject(GameObject* obj) {
    if (!obj) { // Null check to prevent crashes
        std::cerr << "Error: Attempted to add null object to gameObjects!" << std::endl;
        return;
    }

    if (!obj->isActive()) { // Validate active state before adding
        std::cerr << "Error: Attempted to add inactive object to gameObjects!" << std::endl;
        return;
    }

    gameObjects.emplace_back(obj); // Add valid object
}

// --- Deposit Zone Handling ---

// Spawn deposit zone
void GameState::spawnDepositZone() {
    int gridX, gridY;
    DepositZone::Shape shape;

    // Random position for deposit zone
    gridX = rand() % 9; // Ensures valid positions for larger shapes
    gridY = rand() % 9;

    // Random shape
    int shapeIndex = rand() % 3;
    switch (shapeIndex) {
    case 0: shape = DepositZone::Shape::STRAIGHT_LINE; break;
    case 1: shape = DepositZone::Shape::DONUT; break;
    case 2: shape = DepositZone::Shape::CIRCLE; break;
    }

    bool horizontal = rand() % 2 == 0; // Horizontal or vertical line

    // Create new deposit zone
    depositZone = std::make_unique<DepositZone>(this, gridX, gridY, shape, horizontal);
}

// Update game state
void GameState::update(float dt) {
    if (isGameOver) return; // Stop updates after player death

    // --- Preallocate memory to avoid resizing ---
    if (gameObjects.capacity() < 100) {
        gameObjects.reserve(100);
    }

    // --- MOVING ENEMY SPAWNING ---
    time_t currentTime = time(nullptr);
    if (difftime(currentTime, lastMovingEnemySpawnTime) >= movingEnemySpawnInterval) {
        spawnInteractiveObject<MovingEnemy>();
        lastMovingEnemySpawnTime = currentTime;
        movingEnemySpawnInterval = movingEnemySpawnMin + (rand() / (RAND_MAX / (movingEnemySpawnMax - movingEnemySpawnMin)));
    }

    // --- STATIONARY ENEMY SPAWNING ---
    if (difftime(currentTime, lastStationarySpawnTime) >= stationaryEnemySpawnInterval) {
        spawnInteractiveObject<StationaryEnemy>();
        lastStationarySpawnTime = currentTime;
        stationaryEnemySpawnInterval = stationarySpawnMin + (rand() / (RAND_MAX / (stationarySpawnMax - stationarySpawnMin)));
    }

    // --- DEPOSIT ZONE HANDLING ---
    if (!depositZone) {
        spawnDepositZone(); // First-time spawn
    }
    else {
        depositZone->update(dt); // Update zone timer

        // Smooth despawn transition when expired
        if (depositZone->isExpired()) {
            std::cout << "Deposit zone expired! Respawning..." << std::endl;

            // Log expiration and clear tiles for debugging
            for (const auto& tile : depositZone->getTiles()) {
                std::cout << "Tile: (" << tile.first << ", " << tile.second << ")" << std::endl;
            }

            // Reset and respawn
            depositZone->resetTimer(); // Reset timer for new zone
            spawnDepositZone();        // Create a new zone immediately
        }
    }

    // --- COLLECTIBLE RESPAWN MANAGEMENT ---
    for (auto it = collectibleRespawnTimers.begin(); it != collectibleRespawnTimers.end();) {
        if (difftime(currentTime, it->second) >= collectibleRespawnDelay) {
            spawnInteractiveObject<Collectible>();
            it = collectibleRespawnTimers.erase(it);
        }
        else {
            ++it;
        }
    }

    // --- POWERUP SPAWNING ---
    if (difftime(currentTime, lastPowerUpSpawnTime) >= powerUpSpawnInterval) {
        spawnInteractiveObject<PowerUpBlue>();
        lastPowerUpSpawnTime = currentTime;
        powerUpSpawnInterval = powerUpSpawnMin + (rand() / (RAND_MAX / (powerUpSpawnMax - powerUpSpawnMin)));
    }

    // --- COLLISION DETECTION ---
    Player* player = nullptr;

    // Locate the player
    for (auto& obj : gameObjects) {
        player = dynamic_cast<Player*>(obj.get());
        if (player) break;
    }

    static int previousTailSize = -1;
    if (player) {
        int currentTailSize = player->getTailSize();
        if (currentTailSize != previousTailSize) {
            std::cout << "Player tail size updated: " << currentTailSize << std::endl;
            previousTailSize = currentTailSize;
        }

        if (depositZone) { // Check tail segments in the deposit zone
            for (const auto& segment : player->tail) {
                if (depositZone->isTileInZone(segment.gridX, segment.gridY)) {
                    std::cout << "Tail segment inside deposit zone: (" << segment.gridX << ", " << segment.gridY << ")" << std::endl;
                }
            }
        }

        // Check collisions
        for (auto& obj : gameObjects) {
            InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
            if (interactive && interactive->isActive() && interactive->checkCollision(*player)) {
                interactive->handleCollision(*player);
            }
        }
    }

    // --- UPDATE AND REMOVE OBJECTS ---
    std::vector<std::unique_ptr<GameObject>> newObjects; // Buffer for new objects

    // Update all objects and buffer new ones
    for (auto& obj : gameObjects) {
        if (obj && obj->isActive()) {
            obj->update(dt);

            // Collect new objects if spawned during update
            auto movingEnemy = dynamic_cast<MovingEnemy*>(obj.get());
            if (movingEnemy && movingEnemy->getIsWeak()) {
                auto newEnemy = std::make_unique<MovingEnemy>(this, movingEnemy->getGridX(), movingEnemy->getGridY());
                if (newEnemy) {
                    newObjects.push_back(std::move(newEnemy));
                }
            }
        }
    }

    // Append buffered objects safely
    for (auto& newObj : newObjects) {
        gameObjects.push_back(std::move(newObj));
    }

    // Remove inactive objects safely
    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const std::unique_ptr<GameObject>& obj) { return !obj->isActive(); }),
        gameObjects.end());

    // --- Debug Logs ---
#ifdef DEBUG
    std::cout << "Active objects after update: " << gameObjects.size() << std::endl;
#endif
}

// Draw all game objects
void GameState::draw() {

    // Draw deposit zone first (part of the board)
    if (depositZone) {
        depositZone->draw();
    }

    // Draw all other active objects
    for (auto& obj : gameObjects) {
        if (obj->isActive()) {
            obj->draw();
        }
    }

    // Locate the player
    Player* player = nullptr;
    for (auto& obj : gameObjects) {
        player = dynamic_cast<Player*>(obj.get());
        if (player) break;
    }

    if (player) {
        // Set up the font and brush for text
        graphics::setFont(std::string("assets/Arial.ttf")); // Use Arial font
        graphics::Brush textBrush;
        textBrush.fill_color[0] = 1.0f; // Red text
        textBrush.fill_color[1] = 0.0f;
        textBrush.fill_color[2] = 0.0f;
        textBrush.outline_opacity = 0.0f;

        // Display tail size
        std::string tailText = "Tail Size: " + std::to_string(player->getTailSize());
        float xPos = (GRID_SIZE * CELL_SIZE) / 2.0f; // Horizontal center
        float yPos = 30;                             // 30 pixels from the top
        graphics::drawText(xPos, yPos, 30, tailText, textBrush); // Size 30

        // Display score below tail size
        std::string scoreText = "Score: " + std::to_string(getScore());
        graphics::drawText(xPos, yPos + 40, 30, scoreText, textBrush); // Offset by 40 pixels

        // Display tally below score
        std::string tallyText = "Tally: " + std::to_string(getTally());
        graphics::drawText(xPos, yPos + 80, 30, tallyText, textBrush); // Offset by 80 pixels
    }

}


// Initialize game state
void GameState::init() {
    lastMovingEnemySpawnTime = time(nullptr);
    movingEnemySpawnInterval = movingEnemySpawnMin + (rand() / (RAND_MAX / (movingEnemySpawnMax - movingEnemySpawnMin)));

    lastStationarySpawnTime = time(nullptr); // Added for stationary enemies
    stationaryEnemySpawnInterval = stationarySpawnMin + (rand() / (RAND_MAX / (stationarySpawnMax - stationarySpawnMin)));

    lastPowerUpSpawnTime = time(nullptr);
    srand(static_cast<unsigned int>(time(nullptr)));

    powerUpSpawnInterval = powerUpSpawnMin + (rand() / (RAND_MAX / (powerUpSpawnMax - powerUpSpawnMin)));

    spawnDepositZone(); // Start with a deposit zone

    // Spawn initial collectibles
    for (int i = 0; i < collectibleCount; i++) {
        spawnInteractiveObject<Collectible>();
    }
}

void GameState::addScore(int points) {
    score += points; // Increment total score
    std::cout << "Score updated! Current score: " << score << std::endl;
}

void GameState::incrementTally(int count) {
    tally += count; // Increment tally by deposits
    std::cout << "Tally updated! Current tally: " << tally << std::endl;

    // Reset tally if 6 or more deposits
    if (tally >= 6) {
        std::cout << "Tally reset! Reached " << tally << " deposits." << std::endl;
        resetTally(); // Reset tally after triggering power-up
    }
}

void GameState::resetTally() {
    tally = 0; // Reset tally to zero
    std::cout << "Tally reset to 0." << std::endl;
}

// Reset game state
void GameState::reset() {}

// Handles stopping activity after player death
void GameState::endGame() {
    isGameOver = true;
    std::cout << "Game Over! All activity stopped." << std::endl;
}

// Schedule collectible respawn
void GameState::scheduleCollectibleRespawn() {
    time_t currentTime = time(nullptr);
    collectibleRespawnTimers.emplace_back(currentTime, currentTime + static_cast<time_t>(collectibleRespawnDelay));
}

// Destructor
GameState::~GameState() {
    gameObjects.clear();
}
