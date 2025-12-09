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
    static void ExecuteCommand(const CmdSetPipeline& c) {
        glUseProgram(c.pipeline);
    }

    static void ExecuteCommand(const CmdBindVertexBuffer& c) {
        glBindBuffer(GL_ARRAY_BUFFER, c.buffer);
    }

    static void ExecuteCommand(const CmdBindTexture& c) {
        glActiveTexture(GL_TEXTURE0 + c.slot);
        glBindTexture(GL_TEXTURE_2D, c.texture);
    }

    static void ExecuteCommand(const CmdDraw& c) {
        glDrawArrays(GL_TRIANGLES, c.firstVertex, c.vertexCount);
    }
};

}
