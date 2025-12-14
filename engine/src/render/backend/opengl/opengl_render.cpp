#include "engine/render/backends/opengl/opengl_render.h"
#include "engine/render/backends/opengl/opengl_shader.h"

namespace engine::render {

OpenGLRenderBackend::OpenGLRenderBackend(
    const std::shared_ptr<ui::UICanvas>& ui
) : _ui(ui) {

}

void OpenGLRenderBackend::Execute(const std::vector<Command> &commands) {
    for (auto& cmd : commands) {
        std::visit([&](auto& c) {
            ExecuteCommand(c);
        }, cmd);
    }
}

void OpenGLRenderBackend::ExecuteCommand(const CmdDrawUI& c) const {
    _ui->Draw(c.uiElement);
}

void OpenGLRenderBackend::ExecuteCommand(const CmdCustomDraw& c) {
    c.draw();
}

void OpenGLRenderBackend::ExecuteCommand(const CmdDrawMesh& c) {
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

}