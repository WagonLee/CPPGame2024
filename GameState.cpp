#include "GameState.h"
#include "DepositZone.h"
#include <iostream>  // Debug output added
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "MovingEnemy.h"
#include "StationaryEnemy.h"
#include "Collectible.h"
#include "Config.h"
#include "PowerUpBase.h"
#include "PowerUpLevel1.h" // Include Level 1 Power-Up
#include "PowerUpLevel2.h" // Include Level 2 Power-Up
#include "PowerUpLevel3.h" // Include Level 3 Power-Up
#include "MenuUtils.h"
#include <algorithm>

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

void GameState::spawnPowerUp(int level) {
    int gridX, gridY;
    bool positionValid = false;

    // Attempt to find a valid position
    for (int attempts = 0; attempts < 100; ++attempts) {
        gridX = rand() % 12; // Random X position
        gridY = rand() % 12; // Random Y position

        // Check if position is valid
        positionValid = true;
        for (const auto& obj : gameObjects) {
            InteractiveObject* interactive = dynamic_cast<InteractiveObject*>(obj.get());
            if (interactive && interactive->isActive() &&
                interactive->getGridX() == gridX && interactive->getGridY() == gridY) {
                positionValid = false;
                break;
            }
        }

        if (positionValid) {
            break; // Exit loop on valid position
        }
    }

    // Abort if no valid position is found
    if (!positionValid) {
        std::cerr << "Failed to find a valid position for Power-Up Level " << level << std::endl;
        return;
    }

    // Create the appropriate power-up
    std::unique_ptr<PowerUpBase> powerUp = nullptr;
    switch (level) {
    case 1:
        powerUp = std::make_unique<PowerUpLevel1>(this, gridX, gridY);
        break;
    case 2:
        powerUp = std::make_unique<PowerUpLevel2>(this, gridX, gridY);
        break;
    case 3:
        powerUp = std::make_unique<PowerUpLevel3>(this, gridX, gridY);
        break;
    }

    // Add the power-up if created successfully
    if (powerUp) {
        powerUp->init(); // Initialize the object
        activePowerUps.push_back(std::move(powerUp));
        addObject(activePowerUps.back().get());

        // Add the index (not pointer) to timers
        size_t index = activePowerUps.size() - 1;
        upgradeTimers.emplace_back(index, 0.0f);

        std::cout << "Added Power-Up Level " << level << " at index " << index << " to timers." << std::endl;
    }
    else {
        std::cerr << "Failed to create Power-Up Level " << level << std::endl;
    }
}

void GameState::spawnPowerUpAt(int level, int gridX, int gridY) {
    std::unique_ptr<PowerUpBase> powerUp = nullptr;

    switch (level) {
    case 1:
        powerUp = std::make_unique<PowerUpLevel1>(this, gridX, gridY);
        break;
    case 2:
        powerUp = std::make_unique<PowerUpLevel2>(this, gridX, gridY);
        break;
    case 3:
        powerUp = std::make_unique<PowerUpLevel3>(this, gridX, gridY);
        break;
    }

    if (powerUp) {
        powerUp->init();
        activePowerUps.push_back(std::move(powerUp));
        addObject(activePowerUps.back().get());

        // Add the new Power-Up to timers
        size_t index = activePowerUps.size() - 1;
        upgradeTimers.emplace_back(index, 0.0f); // Use index instead of pointer
        std::cout << "Upgraded Power-Up Level " << level << " added to timers at ("
            << gridX << ", " << gridY << ")" << std::endl;
    }
    else {
        std::cerr << "Failed to create upgraded Power-Up Level " << level << std::endl;
    }
}

bool GameState::isPowerUpUpgrading(PowerUpBase* powerup) const {
    for (const auto& timer : upgradeTimers) {
        if (timer.first < activePowerUps.size() &&
            activePowerUps[timer.first].get() == powerup) {
            return true;
        }
    }
    return false;
}

void GameState::updatePowerUpTimers(float dt) {
    const float upgradeTime = 200.0f; // Time required for auto-upgrade

    for (size_t i = 0; i < upgradeTimers.size(); ++i) {
        size_t index = upgradeTimers[i].first;
        float& elapsedTime = upgradeTimers[i].second;

        // Ensure the index is valid and the power-up exists
        if (index >= activePowerUps.size()) {
            upgradeTimers.erase(upgradeTimers.begin() + i);
            --i;
            continue;
        }

        PowerUpBase* powerUp = dynamic_cast<PowerUpBase*>(activePowerUps[index].get());

        // Skip invalid or inactive power-ups, or those already collected
        if (!powerUp || !powerUp->isActive() || !powerUp->canCollide()) {
            std::cout << "Removing timer for collected or inactive Power-Up at index " << index << std::endl;
            upgradeTimers.erase(upgradeTimers.begin() + i);
            --i;
            continue;
        }

        // Skip upgrade for Level 3 (max level)
        if (powerUp->getLevel() >= 3) {
            std::cout << "Removing timer for max-level Power-Up at index " << index << std::endl;
            upgradeTimers.erase(upgradeTimers.begin() + i);
            --i;
            continue;
        }

        // Increment elapsed time
        elapsedTime += dt / 60.0f; // Adjust for frame rate scaling
        std::cout << "Timer for Power-Up Level " << powerUp->getLevel()
            << ": " << elapsedTime << "/" << upgradeTime << " seconds" << std::endl;

        // Auto-upgrade if time threshold is reached
        if (elapsedTime >= upgradeTime && powerUp->isActive()) {
            int newLevel = powerUp->getLevel() + 1;

            std::cout << "Auto-upgrading Power-Up at (" << powerUp->getGridX()
                << ", " << powerUp->getGridY() << ") to Level " << newLevel << std::endl;

            // Spawn upgraded power-up
            spawnPowerUpAt(newLevel, powerUp->getGridX(), powerUp->getGridY());

            // Deactivate current power-up
            powerUp->setActive(false);

            // Remove the old timer
            upgradeTimers.erase(upgradeTimers.begin() + i);
            --i;

            // Add a new timer for the upgraded power-up (if not max level)
            if (newLevel < 3) {
                size_t newIndex = activePowerUps.size() - 1;
                upgradeTimers.emplace_back(newIndex, 0.0f);
                std::cout << "Added timer for upgraded Power-Up Level " << newLevel
                    << " at index " << newIndex << std::endl;
            }
        }
    }
}

// Update game state
void GameState::update(float dt) {

    if (isGameOver) return; // Stop updates after player death

    // Handle pre-game pause ("READY?" state)
    if (isPreGamePaused()) {
        if (graphics::getKeyState(graphics::SCANCODE_UP) || graphics::getKeyState(graphics::SCANCODE_W) ||
            graphics::getKeyState(graphics::SCANCODE_DOWN) || graphics::getKeyState(graphics::SCANCODE_S) ||
            graphics::getKeyState(graphics::SCANCODE_LEFT) || graphics::getKeyState(graphics::SCANCODE_A) ||
            graphics::getKeyState(graphics::SCANCODE_RIGHT) || graphics::getKeyState(graphics::SCANCODE_D)) {
            setPreGamePause(false); // Start the game when a movement key is pressed
        }
        return; // Skip updates during the "READY?" state
    }

    // Handle pause state
    if (paused) {
        updatePauseMenu(); // Update pause menu logic
        return; // Skip gameplay updates
    }

    // Allow toggling pause with ESCAPE
    if (graphics::getKeyState(graphics::SCANCODE_P)) {
        setPaused(!isPaused()); // Toggle pause state
    }

    // Handle general pause state (future implementation)
    if (isPaused()) return;

    setProcessingUpdates(true);

    // --- Preallocate memory to avoid resizing ---
    if (gameObjects.capacity() < 100) {
        gameObjects.reserve(100);
    }

    time_t currentTime = time(nullptr); // declaration

    // --- MOVING ENEMY SPAWNING ---
    if (!isAnyPowerUpActive() && difftime(currentTime, lastMovingEnemySpawnTime) >= movingEnemySpawnInterval) {
        spawnInteractiveObject<MovingEnemy>();
        lastMovingEnemySpawnTime = currentTime;
        movingEnemySpawnInterval = movingEnemySpawnMin + (rand() / (RAND_MAX / (movingEnemySpawnMax - movingEnemySpawnMin)));
    }

    // --- STATIONARY ENEMY SPAWNING ---
    if (!isAnyPowerUpActive() && difftime(currentTime, lastStationarySpawnTime) >= stationaryEnemySpawnInterval) {
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

    // --- POWER-UP AUTO-UPGRADE TIMERS ---
    updatePowerUpTimers(dt);

    if (tally >= tallyLevel3) {
        std::cout << "Condition met for Level 3 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(3);
    }
    else if (tally >= tallyLevel2) {
        std::cout << "Condition met for Level 2 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(2);
    }
    else if (tally >= tallyLevel1) {
        std::cout << "Condition met for Level 1 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(1);
    }

    // Reset Tally AFTER checking ALL conditions
    if (tally >= tallyLevel1) { // Only reset if a Power-Up was attempted
        std::cout << "Tally reset! Reached " << tally << " deposits." << std::endl;
        resetTally();
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
        }
    }

    // Remove inactive objects
    gameObjects.erase(
        std::remove_if(
            gameObjects.begin(),
            gameObjects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return !obj || !obj->isActive();
            }),
        gameObjects.end()
                );

    setProcessingUpdates(false);

    setProcessingUpdates(false);

    if (isPowerUpRemovalPending) {
        cleanupMarkedPowerUps();
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

    // Show "READY?" during pre-game pause
    if (isPreGamePaused()) {
        graphics::Brush textBrush;
        textBrush.fill_color[0] = 0.0f; // Green
        textBrush.fill_color[1] = 1.0f;
        textBrush.fill_color[2] = 0.0f;
        textBrush.outline_opacity = 1.0f;

        graphics::drawText(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2, 40, "READY?", textBrush);
    }

    if (paused) {
        drawPauseMenu(); // Draw the pause menu
        return;          // Skip other rendering
    }

    // Draw all game objects
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
    // Initialize timers for spawning enemies
    lastMovingEnemySpawnTime = time(nullptr);
    movingEnemySpawnInterval = movingEnemySpawnMin + (rand() / (RAND_MAX / (movingEnemySpawnMax - movingEnemySpawnMin)));

    lastStationarySpawnTime = time(nullptr);
    stationaryEnemySpawnInterval = stationarySpawnMin + (rand() / (RAND_MAX / (stationarySpawnMax - stationarySpawnMin)));

    srand(static_cast<unsigned int>(time(nullptr)));

    // Spawn a deposit zone
    spawnDepositZone();

    // Spawn initial collectibles
    for (int i = 0; i < collectibleCount; i++) {
        spawnInteractiveObject<Collectible>();
    }

    // Initialize the player
    int startX = GRID_SIZE / 2; // Center of the grid
    int startY = GRID_SIZE / 2;
    Player* player = new Player(this, startX, startY, 0.004f); // Adjust speed as needed
    addObject(player); // Add the player to gameObjects
}

void GameState::replaceDepositZone() {
    std::cout << "Replacing deposit zone immediately!" << std::endl;

    // Clear old zone
    depositZone.reset();

    // Spawn new zone
    spawnDepositZone(); // Use existing spawn logic
}

bool GameState::isAnyPowerUpActive() const {
    for (const auto& powerUp : activePowerUps) {
        if (powerUp->isEffectRunning()) {
            return true; // Active effect detected
        }
    }
    return false; // No active effects
}

void GameState::markPowerUpForRemoval(PowerUpBase* powerUp) {
    if (!powerUp || powerUp->isMarkedForRemoval()) return;

    if (std::find(powerUpsToRemove.begin(), powerUpsToRemove.end(), powerUp) == powerUpsToRemove.end()) {
        powerUpsToRemove.push_back(powerUp);
        isPowerUpRemovalPending = true;
        std::cout << "Power-up marked for future removal" << std::endl;
    }
}

void GameState::cleanupMarkedPowerUps() {
    if (!isPowerUpRemovalPending || isProcessingUpdates) return;

    std::cout << "Starting power-up cleanup process..." << std::endl;

    // Mark power-ups for cleanup
    for (auto* powerUp : powerUpsToRemove) {
        if (powerUp) {
            powerUp->setInCleanup(true); // Prevent recursive cleanup
        }
    }

    // Remove from activePowerUps
    activePowerUps.erase(
        std::remove_if(
            activePowerUps.begin(),
            activePowerUps.end(),
            [](const std::unique_ptr<PowerUpBase>& powerUp) {
                return powerUp && powerUp->isMarkedForRemoval(); // Corrected call
            }),
        activePowerUps.end()
                );

    // Remove from gameObjects
    gameObjects.erase(
        std::remove_if(
            gameObjects.begin(),
            gameObjects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                if (auto* powerUp = dynamic_cast<PowerUpBase*>(obj.get())) {
                    return powerUp->isMarkedForRemoval(); // Corrected call
                }
    return false;
            }),
        gameObjects.end()
                );

    powerUpsToRemove.clear();
    isPowerUpRemovalPending = false;

    std::cout << "Power-up cleanup complete." << std::endl;
}

void GameState::addScore(int points) {
    score += points; // Increment total score
    std::cout << "Score updated! Current score: " << score << std::endl;
}

void GameState::incrementTally(int count) {
    tally += count; // Increment the tally
    std::cout << "Tally updated! Current tally: " << tally << std::endl;
}

void GameState::resetTally() {
    tally = 0; // Reset tally to zero
    std::cout << "Tally reset to 0." << std::endl;
}

// Set the paused state
void GameState::setPaused(bool paused) {
    this->paused = paused; // Update the paused variable
}

// Check if the game is paused
bool GameState::isPaused() const {
    return paused; // Return the paused state
}

// Set the pre-game pause state
void GameState::setPreGamePause(bool preGame) {
    preGamePaused = preGame; // Update the pre-game pause variable
}

// Check if the game is in the pre-game pause state
bool GameState::isPreGamePaused() const {
    return preGamePaused; // Return the pre-game pause state
}

void GameState::updatePauseMenu() {
    static bool selectTriggered = false;

    // Update pause menu selection
    pauseMenuSelection = handleMenuInput(PAUSE_MENU_OPTIONS, pauseMenuSelection, selectTriggered);

    // Handle option selection
    if (selectTriggered) {
        switch (pauseMenuSelection) {
        case 0: // RESUME
            setPaused(false); // Unpause the game
            break;
        case 1: // RESTART
            resetGameStates();
            init();
            setPaused(false); // Resume the game after restarting
            break;
        case 2: // MAIN MENU
            extern bool inMenu;
            inMenu = true; // Transition back to the main menu
            break;
        case 3: // EXIT
            graphics::stopMessageLoop(); // Exit the game
            break;
        }
    }
}

void GameState::drawPauseMenu() {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    // Draw pause menu background
    br.texture = "assets/menu_background.png"; // Same as main menu background
    graphics::drawRect(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT, br);

    // Draw menu options
    const float charWidth = 15.0f; // Approximate width per character for size 30 font
    for (size_t i = 0; i < PAUSE_MENU_OPTIONS.size(); ++i) {
        br.fill_color[0] = 1.0f; // Default white color
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;

        if (i == pauseMenuSelection) {
            br.fill_color[0] = 0.0f; // Highlight selected option in green
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;
        }

        float textWidth = PAUSE_MENU_OPTIONS[i].size() * charWidth; // Approximate text width
        float x = (WINDOW_WIDTH - textWidth) / 2; // Center horizontally
        float y = WINDOW_HEIGHT / 2 + (i - PAUSE_MENU_OPTIONS.size() / 2) * 50; // Vertical alignment

        graphics::drawText(x, y, 30, PAUSE_MENU_OPTIONS[i], br);
    }
}

// Reset all game states
void GameState::resetGameStates() {
    paused = false;         // Reset paused state
    preGamePaused = true;   // Start with pre-game pause
    isGameOver = false;     // Reset game over flag
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
