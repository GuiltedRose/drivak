#pragma once
#include <memory>
#include <vector>
#include "ui/UIElement.h"

class UIScreen {
public:
    virtual ~UIScreen() = default;

    // Marked virtual so draw() can be overridden
    virtual void draw();

    // Add element to screen
    void addElement(std::shared_ptr<UIElement> element);

    // Provide access for subclasses (safe option)
    const std::vector<std::shared_ptr<UIElement>>& getElements() const;

protected:
    // Protected so subclasses can access directly (optional)
    std::vector<std::shared_ptr<UIElement>> elements;
};
