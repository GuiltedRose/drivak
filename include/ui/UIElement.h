#pragma once
#include <string>
#include <glm/vec2.hpp>

class UIElement {
public:
    UIElement();
    virtual ~UIElement() = default;

    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setVisible(bool show);

    virtual void draw() = 0;

    glm::vec2 getPosition() const;
    glm::vec2 getSize() const;
    bool isVisible() const;

protected:
    glm::vec2 position;
    glm::vec2 size;
    bool visible = true;
};
