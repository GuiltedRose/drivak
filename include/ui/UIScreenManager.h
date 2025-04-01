#pragma once
#include <stack>
#include <unordered_map>
#include <string>
#include <memory>
#include "UIScreen.h"

class UIScreenManager {
public:
    void pushScreen(std::shared_ptr<UIScreen> screen);
    void popScreen();
    void setScreen(std::shared_ptr<UIScreen> screen); // Wipes stack, replaces top

    void registerScreen(const std::string& name, std::shared_ptr<UIScreen> screen);
    void switchTo(const std::string& name); // Switches to a registered screen

    void draw();
    std::shared_ptr<UIScreen> getActiveScreen() const;

private:
    std::stack<std::shared_ptr<UIScreen>> screenStack;
    std::unordered_map<std::string, std::shared_ptr<UIScreen>> screenRegistry;
};
