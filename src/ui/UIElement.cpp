#include "ui/UIElement.h"

// This constructor was using `vec2(0.0f)` which doesn't exist
// Update it to use your custom two-arg constructor
UIElement::UIElement() : position(0.0f, 0.0f), size(0.0f, 0.0f), visible(true) {}

vec2 UIElement::getPosition() const {
    return position;
}

vec2 UIElement::getSize() const {
    return size;
}

bool UIElement::isVisible() const {
    return visible;
}

void UIElement::setPosition(float x, float y) {
    position = vec2(x, y);
}

void UIElement::setSize(float width, float height) {
    size = vec2(width, height);
}

void UIElement::setVisible(bool show) {
    visible = show;
}
