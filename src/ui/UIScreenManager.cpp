#include "ui/UIScreenManager.h"

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
