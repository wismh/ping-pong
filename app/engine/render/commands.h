#pragma once
#include "../utils.h"
#include "igpu_mesh.h"
#include "ishader.h"

namespace engine::render {

using TextureHandle  = uint32_t;

struct CmdDrawMesh {
    std::shared_ptr<IShader> shader;
    std::shared_ptr<IMesh> mesh;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

struct CmdCustomDraw {
    std::function<void()> draw;
};

using Command = std::variant<
    CmdDrawMesh,
    CmdCustomDraw
>;

}
