#pragma once

#include "commands.h"
#include "../resources/mesh_desc.h"
#include "../resources/shader_desc.h"

namespace engine::render {

class IGraphicFabric {
public:
    virtual ~IGraphicFabric() = default;

    virtual bool TryCreateMesh(const MeshDesc& desc, std::shared_ptr<IMesh>& mesh, std::string& error) = 0;
    virtual bool TryCreateShader(const ShaderDesc& desc, std::shared_ptr<IShader>& shader, std::string& error) = 0;
};

}
