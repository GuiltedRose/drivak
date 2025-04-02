#include "ui/UIScreenManager.h"
#include <QKeyEvent>
#include <iostream>

void UIScreenManager::pushScreen(std::shared_ptr<UIScreen> screen) {
    screenStack.push(std::move(screen));
}

void UIScreenManager::popScreen() {
    if (!screenStack.empty()) {
        screenStack.pop();
    }
}

void UIScreenManager::setScreen(std::shared_ptr<UIScreen> screen) {
    while (!screenStack.empty()) screenStack.pop();
    screenStack.push(std::move(screen));
}

void UIScreenManager::registerScreen(const std::string& name, std::shared_ptr<UIScreen> screen) {
    screenRegistry[name] = std::move(screen);
}

void UIScreenManager::switchTo(const std::string& name) {
    auto it = screenRegistry.find(name);
    if (it != screenRegistry.end()) {
        setScreen(it->second);
    }
}

void UIScreenManager::draw() {
    if (!screenStack.empty()) {
        screenStack.top()->draw();
    }
}

std::shared_ptr<UIScreen> UIScreenManager::getActiveScreen() const {
    if (!screenStack.empty()) {
        return screenStack.top();
    }
    return nullptr;
}

void UIScreenManager::handleKeyPress(int key) {
    auto screen = getActiveScreen();
    if (!screen) return;

    // Vim-style movement (to be implemented properly)
    switch (key) {
        case Qt::Key_J: std::cout << "[UI] Vim down\n"; break;
        case Qt::Key_K: std::cout << "[UI] Vim up\n"; break;
        case Qt::Key_H: std::cout << "[UI] Vim left\n"; break;
        case Qt::Key_L: std::cout << "[UI] Vim right\n"; break;
        case Qt::Key_Q: std::cout << "[UI] Vim quit\n"; break;
        case Qt::Key_Return:
        case Qt:: Key_Enter: std::cout << "[UI] Enter key pressed (no focused element yet)\n"; break;
    }
}

void UIScreenManager::handleMouseClick(int x, int y) {
    auto screen = getActiveScreen();
    if (!screen) return;

    std::cout << "[UI] Mouse click at (" << x << ", " << y << ")\n";

    // Example hit detection: let's assume buttons or interactive areas have positions and sizes
    // Iterate through UI elements on the screen (e.g., buttons, panels, etc.)

    for (auto& element : screen->getElements()) {
        if (element->isInside(x, y)) {
            // Handle the click event on this element
            element->onClick();
            return;
        }
    }
}