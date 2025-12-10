#pragma once
#include "../utils.h"
#include "igpu_mesh.h"
#include "ishader.h"

namespace engine::render {

using TextureHandle  = uint32_t;

struct CmdDrawMesh {
    std::shared_ptr<IMesh> mesh;
    glm::mat4x4 model;
};

struct CmdUseShader {
    std::shared_ptr<IShader> shader;
};

using Command = std::variant<
    CmdDrawMesh,
    CmdUseShader
>;

}
