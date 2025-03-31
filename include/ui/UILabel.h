#pragma once
#include "UIElement.h"
#include <string>

class UILabel : public UIElement {
public:
    UILabel(const std::string& text);

    void setText(const std::string& text);
    void draw() override;

private:
    std::string content;
};
