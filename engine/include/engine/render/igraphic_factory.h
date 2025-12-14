#pragma once

#include "commands.h"
#include "itexture.h"
#include "../resources/mesh_desc.h"
#include "../resources/shader_desc.h"
#include "../resources/texture_desc.h"

namespace engine::render {

class IGraphicFactory {
public:
    virtual ~IGraphicFactory() = default;

    virtual bool TryCreateTexture(const TextureDesc& desc, std::shared_ptr<ITexture>& texture, std::string& error) = 0;
    virtual bool TryCreateMesh(const MeshDesc& desc, std::shared_ptr<IMesh>& mesh, std::string& error) = 0;
    virtual bool TryCreateShader(const ShaderDesc& desc, std::shared_ptr<IShader>& shader, std::string& error) = 0;
};

}
