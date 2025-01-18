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
#include "PowerUpLevel4.h" // Include Level 4 Power-Up
#include "MenuUtils.h"
#include <algorithm>

GameState* GameState::instance = nullptr;

// Constructor
GameState::GameState() {
    firstSpawnTime = graphics::getGlobalTime(); // Initialize the spawn timer
    std::cout << "GameState initialized. First spawn time set to: " << firstSpawnTime << std::endl;
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

    if (isGameOver) return; // Stop updates after player death

    // Handle pre-game pause ("READY?" state)
    if (isPreGamePaused()) {
        if (graphics::getKeyState(graphics::SCANCODE_UP) ||
            graphics::getKeyState(graphics::SCANCODE_W) ||
            graphics::getKeyState(graphics::SCANCODE_DOWN) ||
            graphics::getKeyState(graphics::SCANCODE_S) ||
            graphics::getKeyState(graphics::SCANCODE_LEFT) ||
            graphics::getKeyState(graphics::SCANCODE_A) ||
            graphics::getKeyState(graphics::SCANCODE_RIGHT) ||
            graphics::getKeyState(graphics::SCANCODE_D)) {

            setPreGamePause(false); // Exit the pre-game pause state
            graphics::playMusic(ASSET_PATH + "sounds/BGM.mp3", 0.6f, true); // Start background music
        }
        return; // Skip updates during the "READY?" state
    }

    // DEBUG DEBUG START DEBUG DEBUG

    // Debug: Spawn a power-up when the P key is pressed
    if (graphics::getKeyState(graphics::SCANCODE_L)) {
            int randomLevel = 1 + (rand() % 4); // Random level between 1 and 4
            spawnPowerUp(randomLevel);
            std::cout << "Debug: Spawned Power-Up Level " << randomLevel << " using L key." << std::endl;

        }
    
    // DEBUG: Spawn deposit zone with the Z key
    static bool zKeyHeld = false;
    if (graphics::getKeyState(graphics::SCANCODE_Z)) {
        if (!zKeyHeld) {
            replaceDepositZone(); // Replace current deposit zone
            std::cout << "Debug: Spawned a new deposit zone using Z key." << std::endl;
            zKeyHeld = true; // Prevent rapid spamming
        }
    }
    else {
        zKeyHeld = false; // Reset when key is released
    }

    // DEBUG DEBUG END DEBUG DEBUG 

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

    // Draw all other active objects EXCEPT player
    for (auto& obj : gameObjects) {
        Player* potentialPlayer = dynamic_cast<Player*>(obj.get());
        if (obj->isActive() && !potentialPlayer) {  // Skip if it's the player
            obj->draw();
        }
    }

    // Find and draw player LAST so it appears on top
    Player* player = nullptr;
    for (auto& obj : gameObjects) {
        player = dynamic_cast<Player*>(obj.get());
        if (player) {
            player->draw();  // Draw player after everything else
            break;
        }
    }

    // Show "READY?" during pre-game pause
    if (isPreGamePaused()) {
        graphics::Brush textBrush;
        textBrush.fill_color[0] = 0.0f; // Green
        textBrush.fill_color[1] = 1.0f;
        textBrush.fill_color[2] = 0.0f;
        textBrush.outline_opacity = 1.0f;
        graphics::drawText(CANVAS_WIDTH / 2 - 100, CANVAS_HEIGHT / 2, 40, "READY?", textBrush);
    }

    // Handle pause menu
    if (paused) {
        drawPauseMenu(); // Draw the pause menu
        return; // Skip other rendering
    }
}

// Initialize game state
void GameState::init() {
    // Initialize timers for spawning enemies
    
    srand(static_cast<unsigned int>(time(nullptr)));

    // Spawn a deposit zone
    spawnDepositZone();

    // Spawn initial collectibles
    for (int i = 0; i < collectibleCount; i++) {
        spawnInteractiveObject<Collectible>();
    }

    // Initialize the player
    int startX = GRID_WIDTH / 2; // Center of the grid horizontally
    int startY = UI_ROWS_ABOVE + (PLAYABLE_ROWS / 2); // Center of the playable area vertically
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
            setPaused(false);       // Unpause the game
            setPreGamePause(true);  // Enter the "READY?" state
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
    br.texture = ASSET_PATH + "menu_background.png"; // Same background as main menu
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

    // Draw pause menu options
    const float fontSize = 30.0f; // Font size
    const float spacing = 50.0f;  // Vertical spacing between options
    const float charWidth = 15.0f; // Approximate width per character for font size 30

    for (size_t i = 0; i < PAUSE_MENU_OPTIONS.size(); ++i) {
        br.fill_color[0] = 1.0f; // Default white color
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;

        if (i == pauseMenuSelection) {
            br.fill_color[0] = 0.0f; // Highlighted color (green)
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;
        }

        // Calculate horizontal and vertical positioning
        float textWidth = PAUSE_MENU_OPTIONS[i].size() * charWidth; // Approximate text width
        float x = (CANVAS_WIDTH - textWidth) / 2; // Center horizontally
        float y = (CANVAS_HEIGHT / 2) + (i - PAUSE_MENU_OPTIONS.size() / 2.0f) * spacing; // Vertical alignment

        // Draw the pause menu option
        graphics::drawText(x, y, fontSize, PAUSE_MENU_OPTIONS[i], br);
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
    isGameOver = true; // Set game over flag

    // Stop current background music
    graphics::stopMusic();

    // Play death music
    graphics::playMusic(ASSET_PATH + "sounds/death.wav", 0.85f, false);

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