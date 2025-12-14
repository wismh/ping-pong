#include "engine/nodes/node_ui.h"

namespace engine {

NodeUI::NodeUI(
    const std::shared_ptr<EventBus>& eventBus,
    const std::shared_ptr<render::CommandBuffer>& commandBuffer,
    const std::shared_ptr<ui::UIElement>& uiElement
):
    _commandBuffer(commandBuffer),
    _uiElement(uiElement)
{
    eventBus->Subscribe<MouseEvent>([this](const MouseEvent& e){
        HandleMouseEvent(e);
    });
}

void NodeUI::UpdateSelf() {
    _commandBuffer->Push(render::CmdDrawUI{
        _uiElement
    });
}

void NodeUI::HandleMouseEvent(const MouseEvent& e) const {
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

}