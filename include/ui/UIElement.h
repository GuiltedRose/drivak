#pragma once
#include <string>
#include "core/vec2.h"

class UIElement {
public:
    UIElement();
    virtual ~UIElement() = default;

    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setVisible(bool show);

    virtual void draw() = 0;

    vec2 getPosition() const;
    vec2 getSize() const;
    bool isVisible() const;

protected:
    vec2 position;
    vec2 size;
    bool visible = true;
};
