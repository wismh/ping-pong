#pragma once
#include <memory>

#include "engine/render/itexture.h"
#include "engine/render/igpu_mesh.h"
#include "engine/render/ishader.h"

namespace engine::ecs {

struct Renderable {
    std::shared_ptr<render::IMesh> mesh;
    std::shared_ptr<render::IShader> shader;
    std::shared_ptr<render::ITexture> texture;
};

}
