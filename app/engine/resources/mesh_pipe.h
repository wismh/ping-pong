#pragma once
#include "mesh_desc.h"
#include "resource_pipe.h"
#include "../render/commands.h"

namespace engine {

class MeshPipe final : public ResourcePipe<MeshDesc, render::IMesh> {
    std::shared_ptr<render::IGraphicFabric> _graphicFabric;
public:
    explicit MeshPipe(
        const std::shared_ptr<render::IGraphicFabric>& graphicFabric
    ) : _graphicFabric(graphicFabric) {

    }

    std::shared_ptr<render::IMesh> Load(const std::string &id, MeshDesc source) override {
        std::shared_ptr<render::IMesh> mesh;
        std::string errorMessage;

        if (!_graphicFabric->TryCreateMesh(source, mesh, errorMessage)) {
            std::cout << "Error:" << errorMessage << std::endl;
            return nullptr;
       }

        _resources.insert(std::make_pair(id, mesh));
        return mesh;
    }

    void Unload(const std::string &id) override {
        if (!_resources.contains(id))
            return;

        auto resource = _resources.at(id);

        // TODO: _graphicFabric->DeleteVertexBuffer()

        _resources.erase(id);
    }
};

}
