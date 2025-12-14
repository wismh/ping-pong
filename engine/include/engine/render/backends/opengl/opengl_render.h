#pragma once

#include "engine/ui/ui_canvas.h"
#include "engine/render/irender_backend.h"

namespace engine::render {

class OpenGLRenderBackend final : public IRenderBackend {
    std::shared_ptr<ui::UICanvas> _ui;
public:
    explicit OpenGLRenderBackend(const std::shared_ptr<ui::UICanvas>& ui);
    void Execute(const std::vector<Command> &commands) override;
private:
    void ExecuteCommand(const CmdDrawUI& c) const;
    static void ExecuteCommand(const CmdCustomDraw& c);
    static void ExecuteCommand(const CmdDrawMesh& c);
};

}
