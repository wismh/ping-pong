#pragma once

#include "../../../ui/ui_canvas.h"
#include "engine/render/irender_backend.h"
#include "opengl_shader.h"

namespace engine::render {

class OpenGLRenderBackend final : public IRenderBackend {
    std::shared_ptr<ui::UICanvas> _ui;
public:
    explicit OpenGLRenderBackend(
        const std::shared_ptr<ui::UICanvas>& ui
    ) : _ui(ui) {

    }

    void Execute(const std::vector<Command> &commands) override {
        for (auto& cmd : commands) {
            std::visit([&](auto& c) {
                ExecuteCommand(c);
            }, cmd);
        }
    }

private:
    void ExecuteCommand(const CmdDrawUI& c) const {
        _ui->Draw(c.uiElement);
    }

    static void ExecuteCommand(const CmdCustomDraw& c) {
        c.draw();
    }

    static void ExecuteCommand(const CmdDrawMesh& c) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        const auto& shader =
            std::dynamic_pointer_cast<OpenGLShader>(c.shader);

        shader->Use();
        shader->SetMat4("uProjection", c.projection);
        shader->SetMat4("uView", c.view);
        shader->SetMat4("uModel", c.model);

        c.texture->Bind();
        c.mesh->Draw();
    }
};

}
