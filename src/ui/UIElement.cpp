#include "ui/UIElement.h"

UIElement::UIElement() : position(0.0f), size(0.0f), visible(true) {}

void UIElement::setPosition(float x, float y) {
    position = {x, y};
}

void UIElement::setSize(float width, float height) {
    size = {width, height};
}

void UIElement::setVisible(bool show) {
    visible = show;
}

glm::vec2 UIElement::getPosition() const {
    return position;
}

glm::vec2 UIElement::getSize() const {
    return size;
}

bool UIElement::isVisible() const {
    return visible;
}
