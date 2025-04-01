#pragma once
#include "ui/UIScreen.h"

class UIScrollScreen : public UIScreen {
public:
    void draw() override;
    void scrollUp();
    void scrollDown();

private:
    int scrollOffset = 0;
};
