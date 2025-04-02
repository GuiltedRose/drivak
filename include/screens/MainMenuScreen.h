#pragma once
#include "ui/UIScreen.h"
#include "ui/UIButton.h"
#include <memory>

class MainMenuScreen : public UIScreen {
public:
    MainMenuScreen();
    void draw() override;
    void onEnter() override;
    void render(VulkanRenderer& renderer) override;
    void handleInput(QKeyEvent* event) override;
    void handleInput(QMouseEvent* event) override;


private:
    std::shared_ptr<UIScreen> screen; 

    // Optional: store direct refs to interactive elements
    std::shared_ptr<UIButton> startButton;
    std::shared_ptr<UIButton> loadButton;
    std::shared_ptr<UIButton> configButton;
};
