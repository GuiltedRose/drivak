#include "ui/UILabel.h"
#include <iostream> // temporary stub for draw()

UILabel::UILabel(const std::string& text) : content(text) {}

void UILabel::setText(const std::string& text) {
    content = text;
}

void UILabel::draw() {
    if (!visible) return;

    // TEMP: replace with Vulkan text rendering
    std::cout << "[UILabel] " << content << " at (" << position.x << ", " << position.y << ")\n";
}
