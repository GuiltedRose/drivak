#pragma once
#include <memory>
#include <vector>
#include <QMouseEvent>
#include <QKeyEvent>
#include "renderer/VulkanRenderer.h"
#include "ui/UIElement.h"

class UIScreen {
    public:
        virtual ~UIScreen() = default;
        
        virtual void draw();
        virtual void onEnter() = 0;
        virtual void render(VulkanRenderer& renderer) = 0;
        virtual void handleInput(QKeyEvent* event) = 0;
        virtual void handleInput(QMouseEvent* event) = 0;
    
        void addElement(std::shared_ptr<UIElement> element);
        const std::vector<std::shared_ptr<UIElement>>& getElements() const;
    
    protected:
        std::vector<std::shared_ptr<UIElement>> elements;
};
    
