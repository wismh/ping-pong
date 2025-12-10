#pragma once
#include "utils.h"

namespace game {

struct Renderable {
    std::shared_ptr<er::IMesh> mesh;
    std::shared_ptr<er::IShader> shader;
    std::shared_ptr<er::ITexture> texture;
};

}
