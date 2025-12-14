#pragma once
#include "engine/render/commands.h"
#include "engine/render/igraphic_fabric.h"

#include "resource_pipe.h"

namespace engine {

class MeshPipe final : public ResourcePipe<render::IMesh> {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<render::IGraphicFabric> _fabric;
public:
    explicit MeshPipe(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<render::IGraphicFabric>& fabric
    );

    std::shared_ptr<render::IMesh> Load(const std::string& path, bool cache) override;
    void Unload(const std::string &source) override;
};

}
