#include "ui/UIButton.h"
#include <iostream> // Temporary stub

UIButton::UIButton(const std::string& text, std::function<void()> onClick)
    : label(text), onClick(onClick) {}

void UIButton::setText(const std::string& text) {
    label = text;
}

void UIButton::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

void UIButton::trigger() {
    if (onClick) onClick();
}

void UIButton::draw() {
    if (!visible) return;

    // TEMP: Replace with styled Vulkan or Qt rendering
    std::cout << "[UIButton] " << label << " at ("
              << position.x << ", " << position.y << ")\n";
}
