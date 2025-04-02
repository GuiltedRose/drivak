#include "screens/StartupScreen.h"
#include "ui/UILabel.h"
#include "ui/UIScreenManager.h"
#include "config/ConfigLoader.h"
#include <iostream>

StartupScreen::StartupScreen() {
    // Set up initial screen content (e.g., labels)
    auto title = new UILabel("Welcome to Drivak");
    title->setPosition(20, 20);
    addElement(title);

    // Load configuration and flags
    loadInitialFlags();
}

void StartupScreen::onEnter() {
    // Implement logic that should happen when the screen is first shown
    std::cout << "[Startup] Entered Startup Screen" << std::endl;
}

void StartupScreen::render(VulkanRenderer& renderer) {
    // Render the elements in the screen
    std::cout << "[Startup] Rendering startup screen" << std::endl;

    // Display popup if needed
    displayPopupIfNeeded(renderer);
}

void StartupScreen::handleInput(QKeyEvent* event) {
    // Delegate the input event handling to UIScreenManager
    UIScreenManager::getInstance().handleKeyPress(event);
}

void StartupScreen::handleInput(QMouseEvent* event) {
    // Delegate mouse event handling to UIScreenManager
    UIScreenManager::getInstance().handleMouseClick(event);
}


void StartupScreen::loadInitialFlags() {
    // Load configuration values related to the screen
    const auto& config = ConfigManager::get();
    
    if (config.getBool("show_roadmap_on_first_launch")) {
        showRoadmap = true;
    }

    if (config.getBool("show_tips")) {
        showTips = true;
    }

    // You can add further flags as required
}

void StartupScreen::displayPopupIfNeeded(VulkanRenderer& renderer) {
    // If the roadmap or tips flag is set, display a popup
    if (showRoadmap) {
        std::string roadmapText = loadRoadmapFile();
        auto roadmapLabel = new UILabel(roadmapText);
        roadmapLabel->setPosition(20, 60);
        roadmapLabel->setWrap(true);
        addElement(roadmapLabel);
    }

    if (showTips) {
        auto tipLabel = new UILabel("Tip: You can edit config.py to customize behavior!");
        tipLabel->setPosition(20, 200);
        tipLabel->setWrap(true);
        addElement(tipLabel);
    }
}

std::string StartupScreen::loadRoadmapFile() {
    std::ifstream file("ROADMAP.md");
    if (!file.is_open()) {
        std::cout << "[Startup] Failed to open ROADMAP.md" << std::endl;
        return "Could not load roadmap.";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
