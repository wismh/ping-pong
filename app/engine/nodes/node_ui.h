#pragma once
#include "../ui/ui_element.h"
#include "node.h"
#include "engine/core/input_system.h"

namespace engine {

class NodeUI final : public Node {
    std::shared_ptr<render::CommandBuffer> _commandBuffer;
    std::shared_ptr<ui::UIElement> _uiElement;
public:
    explicit NodeUI(
        const std::shared_ptr<EventBus>& eventBus,
        const std::shared_ptr<render::CommandBuffer>& commandBuffer,
        const std::shared_ptr<ui::UIElement>& uiElement
    ):
        _commandBuffer(commandBuffer),
        _uiElement(uiElement) {
        eventBus->Subscribe<MouseEvent>([this](const MouseEvent& e){
            HandleMouseEvent(e);
        });
    }
protected:
    void UpdateSelf() override {
        _commandBuffer->Push(render::CmdDrawUI{
            _uiElement
        });
    }
private:
    void HandleMouseEvent(const MouseEvent& e) {
        switch(e.type) {
            case MouseEvent::Type::Down:
                _uiElement->OnMouseDown(e.position);
            break;
            case MouseEvent::Type::Up:
                _uiElement->OnMouseUp(e.position);
            break;
            case MouseEvent::Type::Move:
                _uiElement->OnMouseMove(e.position);
            break;
        }
    }
};

}