#include "ui/UIScreen.h"

void UIScreen::draw() {
    for (const auto& e : elements) {
        e->draw();
    }
}

void UIScreen::addElement(std::shared_ptr<UIElement> element) {
    elements.push_back(std::move(element));
}

const std::vector<std::shared_ptr<UIElement>>& UIScreen::getElements() const {
    return elements;
}
