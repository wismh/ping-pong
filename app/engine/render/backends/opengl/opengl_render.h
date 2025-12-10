#pragma once

#include "opengl_shader.h"
#include "engine/render/irender_backend.h"
#include "glm/gtc/type_ptr.hpp"

namespace engine::render {

class OpenGLRenderBackend final : public IRenderBackend {
public:
    void Execute(const std::vector<Command> &commands) override {
        for (auto& cmd : commands) {
            std::visit([&](auto& c) {
                ExecuteCommand(c);
            }, cmd);
        }
    }

private:
    static void ExecuteCommand(const CmdCustomDraw& c) {
        c.draw();
    }

    static void ExecuteCommand(const CmdDrawMesh& c) {
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
