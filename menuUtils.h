#pragma once
#include <vector>
#include <string>

const std::vector<std::string> MAIN_MENU_OPTIONS = { "PLAY", "HIGH SCORES", "TUTORIAL", "EXIT" };

int handleMenuInput(const std::vector<std::string>& options, int currentSelection, bool& selectTriggered);
