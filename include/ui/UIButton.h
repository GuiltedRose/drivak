#pragma once
#include "UIElement.h"
#include <functional>
#include <string>

class UIButton : public UIElement {
public:
    UIButton(const std::string& text, std::function<void()> onClick);

    void setText(const std::string& text);
    void setOnClick(std::function<void()> callback);

    void draw() override;
    void trigger();  // Simulates clicking the button

private:
    std::string label;
    std::function<void()> onClick;
};
