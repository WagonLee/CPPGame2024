#pragma once
#include <vector>
#include <string>

// Menu options for the main menu
const std::vector<std::string> MAIN_MENU_OPTIONS = { "PLAY", "HIGH SCORES", "TUTORIAL", "EXIT" };

// Menu options for the pause menu
const std::vector<std::string> PAUSE_MENU_OPTIONS = { "RESUME", "RESTART", "MAIN MENU", "EXIT" };

// Handles menu navigation and selection
int handleMenuInput(const std::vector<std::string>& options, int currentSelection, bool& selectTriggered);
