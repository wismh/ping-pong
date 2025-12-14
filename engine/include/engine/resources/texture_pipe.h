#pragma once
#include <spdlog/fmt/bundled/std.h>

#include "engine/render/igraphic_factory.h"
#include "engine/render/itexture.h"

#include "resource_pipe.h"

namespace engine {

class TexturePipe final : public ResourcePipe<render::ITexture> {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<render::IGraphicFactory> _fabric;
public:
    explicit TexturePipe(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<render::IGraphicFactory>& fabric
    );

    std::shared_ptr<render::ITexture> Load(const std::string& path, bool cache) override;
    void Unload(const std::string& path) override;
};

}
