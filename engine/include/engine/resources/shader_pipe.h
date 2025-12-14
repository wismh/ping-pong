#pragma once

#include "engine/render/igraphic_fabric.h"
#include "engine/render/ishader.h"

#include "resource_pipe.h"

namespace engine {

class ShaderPipe final : public ResourcePipe<render::IShader> {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<render::IGraphicFabric> _fabric;
public:
    explicit ShaderPipe(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<render::IGraphicFabric>& fabric
    );

    std::shared_ptr<render::IShader> Load(const std::string& path, bool cache) override;
    void Unload(const std::string &path) override;
};

}
