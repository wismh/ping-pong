#pragma once
#include "../ui/ui_element.h"
#include "node.h"

namespace engine {

class NodeUI final : public Node {
    std::shared_ptr<render::CommandBuffer> _commandBuffer;
    std::shared_ptr<ui::UIElement> _uiElement;
public:
    explicit NodeUI(
        const std::shared_ptr<render::CommandBuffer>& commandBuffer,
        const std::shared_ptr<ui::UIElement>& uiElement
    ):
        _commandBuffer(commandBuffer),
        _uiElement(uiElement) {

    }
protected:
    void UpdateSelf() override {
        _commandBuffer->Push(render::CmdDrawUI{
            _uiElement
        });
    }
};

}