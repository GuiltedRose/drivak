#pragma once

#include "ui/UIScreen.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <string>

class StartupScreen : public UIScreen {
    public:
        StartupScreen();
    
        // Implementing the pure virtual functions
        void onEnter() override;
        void render(VulkanRenderer& renderer) override;
        void handleInput(QKeyEvent* event) override;
        void handleInput(QMouseEvent* event) override;
    
    private:
        bool showRoadmap = false;
        bool showTips = false;
        void loadInitialFlags();
        void displayPopupIfNeeded(VulkanRenderer& renderer);
        std::string loadRoadmapFile;
    };
    