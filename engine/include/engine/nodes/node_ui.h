#pragma once
#include "engine/ui/ui_element.h"
#include "engine/core/input_system.h"
#include "engine/render/command_buffer.h"

#include "node.h"

namespace engine {

class NodeUI final : public Node {
    std::shared_ptr<render::CommandBuffer> _commandBuffer;
    std::shared_ptr<ui::UIElement> _uiElement;
public:
    explicit NodeUI(
        const std::shared_ptr<EventBus>& eventBus,
        const std::shared_ptr<render::CommandBuffer>& commandBuffer,
        const std::shared_ptr<ui::UIElement>& uiElement
    );
protected:
    void UpdateSelf() override;
private:
    void HandleMouseEvent(const MouseEvent& e) const;
};

}