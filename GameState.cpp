#include "gamestate.h"
#include "depositzone.h"
#include <iostream>  // debug output added
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "movingenemy.h"
#include "stationaryenemy.h"
#include "collectible.h"
#include "config.h"
#include "powerupbase.h"
#include "poweruplevel1.h" // include level 1 power-up
#include "poweruplevel2.h" // include level 2 power-up
#include "poweruplevel3.h" // include level 3 power-up
#include "poweruplevel4.h" // include level 4 power-up
#include "menuutils.h"
#include <algorithm>
#include "gridrenderer.h"
#include "hiscoremenu.h"

GameState* GameState::instance = nullptr;

// Constructor
GameState::GameState() {
    firstSpawnTime = graphics::getGlobalTime(); // Initialize the spawn timer
    std::cout << "GameState initialized. First spawn time set to: " << firstSpawnTime << std::endl;
    deathMenuGridState = std::vector<std::vector<Tile>>(
        GRID_HEIGHT,
        std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f))
        );
}

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
    gridX = 1 + (rand() % (PLAYABLE_COLUMNS - 4)); // Ensure space for horizontal lines
    gridY = UI_ROWS_ABOVE + 1 + (rand() % (PLAYABLE_ROWS - 4)); // Ensure space for vertical lines

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
        gridX = 1 + (rand() % PLAYABLE_COLUMNS); // Exclude the 1-grid unplayable column on the left
        gridY = UI_ROWS_ABOVE + (rand() % PLAYABLE_ROWS); // Exclude the top unplayable rows

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
    case 4:
        powerUp = std::make_unique<PowerUpLevel4>(this, gridX, gridY);
        break;
    default:
        std::cerr << "Invalid power-up level: " << level << std::endl;
        return;
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
    case 4:
        powerUp = std::make_unique<PowerUpLevel4>(this, gridX, gridY);
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

        // Skip upgrade for Level 4 (max level)
        if (powerUp->getLevel() >= 4) {
            std::cout << "Removing timer for max-level Power-Up at index " << index << std::endl;
            upgradeTimers.erase(upgradeTimers.begin() + i);
            --i;
            continue;
        }

        // Increment elapsed time
        elapsedTime += dt / 60.0f; // Adjust for frame rate scaling
        /*std::cout << "Timer for Power-Up Level " << powerUp->getLevel()
            << ": " << elapsedTime << "/" << upgradeTime << " seconds" << std::endl;*/

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
            if (newLevel < 4) {
                size_t newIndex = activePowerUps.size() - 1;
                upgradeTimers.emplace_back(newIndex, 0.0f);
                std::cout << "Added timer for upgraded Power-Up Level " << newLevel
                    << " at index " << newIndex << std::endl;
            }
        }
    }

    // Check if any power-up effects are active
    bool powerUpActive = isAnyPowerUpActive();

    if (!powerUpActive) {
        // No active power-ups, reset the kill chain
        resetKillChain();
    }
}

// Update game state
void GameState::update(float dt) {

    if (isGameOver) {
        if (waitingForDeathMenuInput) {
            // Wait for space or X key to transition to the death menu
            if (graphics::getKeyState(graphics::SCANCODE_SPACE) || graphics::getKeyState(graphics::SCANCODE_X) || graphics::getKeyState(graphics::SCANCODE_RETURN) || (graphics::getKeyState(graphics::SCANCODE_UP) ||
                graphics::getKeyState(graphics::SCANCODE_DOWN) ||
                graphics::getKeyState(graphics::SCANCODE_LEFT) ||
                graphics::getKeyState(graphics::SCANCODE_RIGHT))) {
                waitingForDeathMenuInput = false;
                HiScoreMenu::getInstance()->updateLeaderboard(score);
                deathMenuGridState = std::vector<std::vector<Tile>>(GRID_HEIGHT, std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f)));
            }
        }
        else {
            updateDeathMenu(); // Update death menu navigation
        }
        return; // Skip game updates during game over
    }

    // Handle pre-game pause 
    if (isPreGamePaused()) {
        if (graphics::getKeyState(graphics::SCANCODE_UP) ||
            graphics::getKeyState(graphics::SCANCODE_DOWN) ||
            graphics::getKeyState(graphics::SCANCODE_LEFT) ||
            graphics::getKeyState(graphics::SCANCODE_RIGHT)) {

            setPreGamePause(false); // Exit the pre-game pause state
            graphics::playMusic(ASSET_PATH + "sounds/BGM.mp3", 0.6f, true); // Start background music
        }
        return;
    }

    setProcessingUpdates(true);

    // --- Preallocate memory to avoid resizing ---
    if (gameObjects.capacity() < 100) {
        gameObjects.reserve(100);
    }

    time_t currentTime = time(nullptr); // declaration

    // --- MOVING ENEMY SPAWNING
    if (isAnyPowerUpActive()) {
        // Check for inactive enemies and turn them weak
        for (const auto& obj : gameObjects) {
            MovingEnemy* movingEnemy = dynamic_cast<MovingEnemy*>(obj.get());
            if (movingEnemy && movingEnemy->isInactive && !movingEnemy->getIsWeak()) {
                movingEnemy->setWeak(true); // Turn weak if inactive and power-up is active
                std::cout << "Inactive MovingEnemy at (" << movingEnemy->getGridX()
                          << ", " << movingEnemy->getGridY() << ") turned WEAK due to power-up." << std::endl;
            }
        }
    } else {
        // Continue spawning logic if no power-up is active
        if (firstSpawn) {
            // Wait 4 seconds before the first spawn
            if (graphics::getGlobalTime() >= firstSpawnTime + 4000.0f) {
                spawnInteractiveObject<MovingEnemy>();
                firstSpawn = false;
                enemySpawnedInactive = true;
                std::cout << "First MovingEnemy spawned after 4 seconds." << std::endl;
            }
        } else {
            // Spawn new enemy as soon as the last one exits inactive state
            for (const auto& obj : gameObjects) {
                MovingEnemy* movingEnemy = dynamic_cast<MovingEnemy*>(obj.get());
                if (movingEnemy && movingEnemy->isActive() && !enemySpawnedInactive) {
                    spawnInteractiveObject<MovingEnemy>();
                    enemySpawnedInactive = true;
                    std::cout << "New MovingEnemy spawned after previous became active." << std::endl;
                    break;
                }
            }
        }

        // Check if all active enemies have exited inactive state
        bool allActive = true;
        for (const auto& obj : gameObjects) {
            MovingEnemy* movingEnemy = dynamic_cast<MovingEnemy*>(obj.get());
            if (movingEnemy && movingEnemy->isInactive) {
                allActive = false;
                break;
            }
        }

        // Reset enemy spawn flag if all are active
        if (allActive) {
            enemySpawnedInactive = false;
        }
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
            // --- STATIONARY ENEMY SPAWNING ---
            if (!isAnyPowerUpActive()) {
                spawnInteractiveObject<StationaryEnemy>();
                graphics::playSound(ASSET_PATH + "sounds/nodepo.wav", 0.40f, false);
            }

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

    if (tally >= tallyLevel4) {
        std::cout << "Condition met for Level 4 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(4);
        graphics::playSound(ASSET_PATH + "sounds/lvl4.wav", 1.0f, false);
    }
    else if (tally >= tallyLevel3) {
        std::cout << "Condition met for Level 3 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(3);
        graphics::playSound(ASSET_PATH + "sounds/lvl3.wav", 1.0f, false);
    }
    else if (tally >= tallyLevel2) {
        std::cout << "Condition met for Level 2 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(2);
        graphics::playSound(ASSET_PATH + "sounds/lvl2.wav", 1.0f, false);
    }
    else if (tally >= tallyLevel1) {
        std::cout << "Condition met for Level 1 Power-Up. Tally = " << tally << std::endl;
        spawnPowerUp(1);
        graphics::playSound(ASSET_PATH + "sounds/lvl1.wav", 1.0f, false);
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
    // Draw deposit zone first (background layer)
    if (depositZone) {
        depositZone->draw();
    }

    // Draw all other active objects, including collectibles
    for (auto& obj : gameObjects) {
        Player* potentialPlayer = dynamic_cast<Player*>(obj.get());
        if (obj->isActive() && !potentialPlayer) { // Skip player for now
            obj->draw();
        }
    }

    // Draw the player last so it appears on top
    Player* player = nullptr;
    for (auto& obj : gameObjects) {
        player = dynamic_cast<Player*>(obj.get());
        if (player) {
            player->draw(); // Always draw the player regardless of the game state
            break;
        }
    }

    // Check if the player is dead and waiting for input
    if (isGameOver) {
        if (!waitingForDeathMenuInput) {
            drawDeathMenu(); // Render the death menu after input
        }
    }
}



// Initialize game state
void GameState::init() {
    std::cout << "Initializing GameState..." << std::endl;

    // Reset random seed for consistent randomness in each session
    srand(static_cast<unsigned int>(time(nullptr)));

    // Clear existing game objects to ensure a clean slate
    gameObjects.clear();
    activePowerUps.clear();
    collectibleRespawnTimers.clear();
    upgradeTimers.clear();
    powerUpsToRemove.clear();

    // Reset flags and state variables
    paused = false;
    preGamePaused = true;  // Start in the "READY?" state
    isGameOver = false;
    waitingForDeathMenuInput = true;  // Prevent immediate death menu display
    firstSpawn = true;
    enemySpawnedInactive = false;

    // Reset score and tally
    score = 0;
    tally = 0;
    killChain = 0;
    killChainScore = 0;
    scoreMulti = 1;

    // Reset first spawn timer
    firstSpawnTime = graphics::getGlobalTime();

    // Initialize deposit zone
    spawnDepositZone();

    // Spawn initial collectibles
    for (int i = 0; i < collectibleCount; i++) {
        spawnInteractiveObject<Collectible>();
    }

    // Initialize the player at the center of the playable grid
    int startX = GRID_WIDTH / 2; // Horizontal center
    int startY = UI_ROWS_ABOVE + (PLAYABLE_ROWS / 2); // Vertical center
    Player* player = new Player(this, startX, startY, 0.004f); // Adjust speed if needed
    addObject(player); // Add player to game objects

    // Initialize death menu grid state
    deathMenuGridState = std::vector<std::vector<Tile>>(
        GRID_HEIGHT,
        std::vector<Tile>(GRID_WIDTH, Tile(0.0f, 0.0f, 0.0f))
        );

    std::cout << "GameState initialized successfully with a clean state." << std::endl;
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
                return powerUp && powerUp->isMarkedForRemoval();
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

void GameState::resetKillChain() {
    killChain = 0;
    killChainScore = 0;
}

void GameState::addToKillChain() {
    killChain++;
    int points = 10 * killChain * scoreMulti; // Points based on chain and multiplier
    killChainScore += points;                // Add to cumulative score
    addScore(points);
    std::cout << "Kill chain: " << killChain << ", Points awarded: " << points << ", Total: " << killChainScore << std::endl;
}

// Increment the multiplier
void GameState::incrementMultiplier() {
    scoreMulti++;
    std::cout << "Score multiplier increased to " << scoreMulti << "." << std::endl;
}

// Get the current multiplier
int GameState::getMultiplier() const {
    return scoreMulti;
}

// Set the pre-game pause state
void GameState::setPreGamePause(bool preGame) {
    preGamePaused = preGame; // Update the pre-game pause variable
}

// Check if the game is in the pre-game pause state
bool GameState::isPreGamePaused() const {
    return preGamePaused; // Return the pre-game pause state
}

void GameState::clearGrid(std::vector<std::vector<Tile>>& grid) {
    for (auto& row : grid) {
        for (auto& tile : row) {
            tile = Tile(0.0f, 0.0f, 0.0f); // Black background
        }
    }
}

void GameState::drawTitle(std::vector<std::vector<Tile>>& grid, const std::vector<std::string>& title, int row) {
    int startCol = (GRID_WIDTH - static_cast<int>(title.size())) / 2;
    for (size_t i = 0; i < title.size(); ++i) {
        grid[row][startCol + i] = Tile(1.0f, 1.0f, 1.0f);
        grid[row][startCol + i].texture = ASSET_PATH + "chars/" + title[i];
    }
}

void GameState::drawOptions(std::vector<std::vector<Tile>>& grid, const std::vector<std::vector<std::string>>& options, int startRow, int selectedIndex) {
    for (size_t i = 0; i < options.size(); ++i) {
        int startCol = (GRID_WIDTH - static_cast<int>(options[i].size())) / 2;
        for (size_t j = 0; j < options[i].size(); ++j) {
            grid[startRow][startCol + j] = Tile(i == selectedIndex ? 0.0f : 1.0f, 1.0f, i == selectedIndex ? 0.0f : 1.0f); // Highlight selected
            grid[startRow][startCol + j].texture = ASSET_PATH + "chars/" + options[i][j];
        }
        startRow += 2; // Add spacing between options
    }
}

void GameState::drawDeathMenu() {
    clearGrid(deathMenuGridState); // Clear the death menu grid

    // Add Game Over Title
    const std::vector<std::string> title = { "PWND.png", "D.png", "E.png", "A.png", "D.png", "PWND.png" };
    drawTitle(deathMenuGridState, title, 2);

    // Define menu options
    const std::vector<std::vector<std::string>> options = {
        {"R.png", "E.png", "B.png", "O.png", "O.png", "T.png"},   // REBOOT
        {"M.png", "A.png", "I.png", "N.png"},                    // MAIN
        {"E.png", "X.png", "I.png", "T.png"}                     // EXIT
    };

    drawOptions(deathMenuGridState, options, 6, deathMenuSelection);

    // Render the grid
    graphics::Brush br;
    for (int r = 0; r < GRID_HEIGHT; ++r) {
        for (int c = 0; c < GRID_WIDTH; ++c) {
            const auto& tile = deathMenuGridState[r][c];
            br.fill_color[0] = tile.r;
            br.fill_color[1] = tile.g;
            br.fill_color[2] = tile.b;
            br.texture = tile.texture.empty() ? "" : tile.texture;
            br.outline_opacity = 0.0f;

            float x = c * CELL_SIZE + CELL_SIZE / 2;
            float y = r * CELL_SIZE + CELL_SIZE / 2;
            graphics::drawRect(x, y, CELL_SIZE, CELL_SIZE, br);
        }
    }
}

void GameState::updateDeathMenu() {
    static bool selectTriggered = false;

    // Check if the score is a new hiScore
    updateHiScore(score);

    // Update the leaderboard with the current score
    HiScoreMenu::getInstance()->updateLeaderboard(score);

    // Play sound effects for navigation
    static int previousSelection = deathMenuSelection;
    deathMenuSelection = handleMenuInput({ "REBOOT", "MAIN", "EXIT" }, deathMenuSelection, selectTriggered);

    if (deathMenuSelection != previousSelection) {
        if (deathMenuSelection > previousSelection) {
            graphics::playSound(ASSET_PATH + "sounds/down.wav", 0.5f, false); // Down navigation sound
        }
        else {
            graphics::playSound(ASSET_PATH + "sounds/up.wav", 0.5f, false); // Up navigation sound
        }
        previousSelection = deathMenuSelection; // Update the previous selection
    }

    // Handle option selection
    if (selectTriggered) {
        graphics::playSound(ASSET_PATH + "sounds/select.wav", 0.5f, false); // Selection sound

        switch (deathMenuSelection) {
        case 0: // REBOOT
            resetGameStates();
            init(); // Restart game
            setPreGamePause(true); // Enter pre-game pause
            break;

        case 1: // MAIN
            extern bool inMenu;
            inMenu = true; // Return to main menu
            break;

        case 2: // EXIT
            graphics::stopMessageLoop(); // Exit the game
            break;
        }
    }
}


void GameState::updateHiScore(int score) {
    if (score > hiScore) {
        hiScore = score; // Update hiScore if the current score is higher
        std::cout << "New hiScore achieved: " << hiScore << std::endl;
    }
}

// Reset all game states
void GameState::resetGameStates() {
    // Clear all game objects
    gameObjects.clear();
    activePowerUps.clear();
    collectibleRespawnTimers.clear();
    upgradeTimers.clear();
    powerUpsToRemove.clear();

    // Reset player and deposit zone
    depositZone.reset();

    // Reset flags and variables
    paused = false;
    preGamePaused = true;
    isGameOver = false;
    waitingForDeathMenuInput = true;
    firstSpawn = true;
    enemySpawnedInactive = false;

    // Reset scores and counters
    score = 0;
    tally = 0;
    killChain = 0;
    killChainScore = 0;
    scoreMulti = 1;

    // Reset timers
    firstSpawnTime = graphics::getGlobalTime();

    // Clear visual grid states
    if (!deathMenuGridState.empty()) {
        deathMenuGridState.clear();
    }

    std::cout << "GameState fully reset. Ready for reinitialization." << std::endl;
}

// Reset game state
void GameState::reset() {}

// Handles stopping activity after player death
void GameState::endGame() {
    isGameOver = true; // Mark game over
    waitingForDeathMenuInput = true; // Wait for input to open the death menu

    // Update hiScore if the score is higher
    updateHiScore(score);

    // Update HiScoreMenu leaderboard
    auto* hiScoreMenu = HiScoreMenu::getInstance();
    hiScoreMenu->updateLeaderboard(score);

    // Stop current background music
    graphics::stopMusic();

    // Play death music
    graphics::playMusic(ASSET_PATH + "sounds/death.wav", 0.85f, false);

    std::cout << "Game Over! Waiting for input to continue." << std::endl;
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