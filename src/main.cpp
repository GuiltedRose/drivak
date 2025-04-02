#include "gamedb/GameDB.h"
#include "modding/ModLoader.h"
#include "core/DataPaths.h"
#include "ui/UIScreenManager.h"
#include "screens/MainMenuScreen.h"
#include "screens/StartupScreen.h"

#include <iostream>
#include <QCoreApplication>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the screen manager
    auto screenManager = std::make_shared<UIScreenManager>();

    // Register the startup screen
    auto startup = std::make_shared<StartupScreen>();
    screenManager->registerScreen("startup", startup);

    // Register the main menu screen
    auto mainMenu = std::make_shared<MainMenuScreen>();
    screenManager->registerScreen("main_menu", mainMenu);

    // Show startup screen first
    screenManager->switchTo("startup");

    // Optional: wire input handlers here too
    // window->onKeyPress = [&](int key) { screenManager->handleKeyPress(key); };

    // Main draw loop (replace with Vulkan later)
    screenManager->draw();

    return app.exec();
}
