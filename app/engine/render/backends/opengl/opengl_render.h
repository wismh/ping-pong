#pragma once

#include "engine/render/irender_backend.h"

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
    static void ExecuteCommand(const CmdUseShader& c) {
        c.shader->Use();
    }

    static void ExecuteCommand(const CmdDrawMesh& c) {
        c.mesh->Draw();
    }
};

}
