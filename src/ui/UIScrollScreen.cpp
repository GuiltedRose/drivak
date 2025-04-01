#include "ui/UIScrollScreen.h"

void UIScrollScreen::draw() {
    const int visibleLimit = 10;
    int index = 0;

    for (const auto& e : getElements()) {
        if (index >= scrollOffset && index < scrollOffset + visibleLimit) {
            e->draw();
        }
        index++;
    }
}

void UIScrollScreen::scrollUp() {
    if (scrollOffset > 0) scrollOffset--;
}

void UIScrollScreen::scrollDown() {
    if (scrollOffset < static_cast<int>(getElements().size()) - 1) {
        scrollOffset++;
    }
}
