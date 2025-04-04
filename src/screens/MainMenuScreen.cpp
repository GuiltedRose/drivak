#include "screens/MainMenuScreen.h"
#include "ui/UILabel.h"
#include "ui/UIButton.h"
#include <iostream>

MainMenuScreen::MainMenuScreen() {
    auto title = std::make_shared<UILabel>("Drivak: DM Toolkit");
    title->setPosition(100, 40);
    screen.addElement(title);

    auto subtitle = std::make_shared<UILabel>("Begin your session or explore mods.");
    subtitle->setPosition(100, 70);
    screen.addElement(subtitle);

    startButton = std::make_shared<UIButton>("Start New Session", []() {
        std::cout << "[UI] Starting new session...\n";
    });
    startButton->setPosition(100, 120);
    screen.addElement(startButton);

    loadButton = std::make_shared<UIButton>("Load Session", []() {
        std::cout << "[UI] Loading session...\n";
    });
    loadButton->setPosition(100, 160);
    screen.addElement(loadButton);

    configButton = std::make_shared<UIButton>("Open Config Folder", []() {
        std::cout << "[UI] Opening config folder...\n";
    });
    configButton->setPosition(100, 200);
    screen.addElement(configButton);
}

void MainMenuScreen::draw() {
    screen.draw();
}
