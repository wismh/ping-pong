#pragma once
#include "../../igraphic_factory.h"

namespace engine::render {

class OpenGLFactory final : public IGraphicFactory {
public:
    bool TryCreateMesh(
        const MeshDesc& desc,
        std::shared_ptr<IMesh>& mesh,
        std::string& error
    ) override;

    bool TryCreateShader(
        const ShaderDesc& desc,
        std::shared_ptr<IShader>& shader,
        std::string& error
    ) override;

    bool TryCreateTexture(
        const TextureDesc &desc,
        std::shared_ptr<ITexture> &texture,
        std::string &error)
    override;
};

}
