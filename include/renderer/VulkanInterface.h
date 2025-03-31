#pragma once
#include "core/ThemeConfig.h"

class VulkanInterface {
public:
    bool init();                 // Initialize Vulkan instance + surface
    void drawFrame();           // Draw a frame (clears background for now)
    void shutdown();            // Cleanup

    void setTheme(const ThemeConfig& theme);  // Apply visual theming

private:
    // Add Vulkan instance, device, swapchain, etc. here
    // Placeholder for background clear color
    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
};
