#include "engine/render/backends/opengl/opengl_fabric.h"

#include "engine/render/backends/opengl/opengl_mesh.h"
#include "engine/render/backends/opengl/opengl_shader.h"
#include "engine/render/backends/opengl/opengl_texture.h"

namespace engine::render {

bool OpenGLFabric::TryCreateMesh(
   const MeshDesc& desc,
   std::shared_ptr<IMesh>& mesh,
   std::string& error
) {
    try {
        mesh = std::make_shared<OpenGLMesh>(desc);
        return true;
    } catch (std::exception& ex) {
        error = ex.what();
        return false;
    }
}

bool OpenGLFabric::TryCreateShader(
    const ShaderDesc& desc,
    std::shared_ptr<IShader>& shader,
    std::string& error
) {
    try {
        shader = std::make_shared<OpenGLShader>(desc);
        return true;
    } catch (std::exception& ex) {
        error = ex.what();
        return false;
    }
}

bool OpenGLFabric::TryCreateTexture(
    const TextureDesc &desc,
    std::shared_ptr<ITexture> &texture,
    std::string &error
) {
    try {
        texture = std::make_shared<OpenGLTexture>(desc);
        return true;
    } catch (std::exception& ex) {
        error = ex.what();
        return false;
    }
}

}