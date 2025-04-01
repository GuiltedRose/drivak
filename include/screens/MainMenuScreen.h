#pragma once
#include "ui/UIScreen.h"
#include "ui/UIButton.h"
#include <memory>

class MainMenuScreen : public UIScreen {
public:
    MainMenuScreen();
    void draw();

private:
    UIScreen screen;

    // Optional: store direct refs to interactive elements
    std::shared_ptr<UIButton> startButton;
    std::shared_ptr<UIButton> loadButton;
    std::shared_ptr<UIButton> configButton;
};
