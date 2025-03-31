#include "renderer/VulkanInterface.h"
#include <iostream>

bool VulkanInterface::init() {
    // Placeholder logic for Vulkan init
    std::cout << "[Vulkan] Initialized VulkanInterface.\n";
    return true;
}

void VulkanInterface::drawFrame() {
    // Clear the screen with theme background color (stub for now)
    std::cout << "[Vulkan] Drawing frame with clear color: "
              << clearColor[0] << ", " << clearColor[1] << ", "
              << clearColor[2] << ", " << clearColor[3] << "\n";
}

void VulkanInterface::shutdown() {
    std::cout << "[Vulkan] Shutting down Vulkan.\n";
}

void VulkanInterface::setTheme(const ThemeConfig& theme) {
    // Convert hex color (e.g. "#1c1c1c") to float RGBA
    if (theme.background_color.length() == 7 && theme.background_color[0] == '#') {
        int r = std::stoi(theme.background_color.substr(1, 2), nullptr, 16);
        int g = std::stoi(theme.background_color.substr(3, 2), nullptr, 16);
        int b = std::stoi(theme.background_color.substr(5, 2), nullptr, 16);

        clearColor[0] = r / 255.0f;
        clearColor[1] = g / 255.0f;
        clearColor[2] = b / 255.0f;
        clearColor[3] = 1.0f;
    }

    std::cout << "[Vulkan] Theme applied (bg color): " << theme.background_color << "\n";
}
