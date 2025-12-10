#pragma once
#include "opengl_mesh.h"
#include "opengl_shader.h"
#include "../../igraphic_fabric.h"

namespace engine::render {

class OpenGLFabric final : public IGraphicFabric {
public:
    bool TryCreateMesh(
        const MeshDesc& desc,
        std::shared_ptr<IMesh>& mesh,
        std::string& error
    ) override {
        try {
            mesh = std::make_shared<OpenGLMesh>(desc);
            return true;
        } catch (std::exception& ex) {
            error = ex.what();
            return false;
        }
    }

    bool TryCreateShader(
        const ShaderDesc& desc,
        std::shared_ptr<IShader>& shader,
        std::string& error
    ) override {
        try {
            shader = std::make_shared<OpenGLShader>(desc);
            return true;
        } catch (std::exception& ex) {
            error = ex.what();
            return false;
        }
    }
};

}
