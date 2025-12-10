#pragma once
#include <fstream>

#include "mesh_desc.h"
#include "resource_pipe.h"
#include "../render/commands.h"

namespace engine {

class MeshPipe final : public ResourcePipe<const std::string&, render::IMesh> {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<render::IGraphicFabric> _fabric;
public:
    explicit MeshPipe(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<render::IGraphicFabric>& fabric
    ) :
        _logger(logger->Get()),
        _fabric(fabric) {

    }

    std::shared_ptr<render::IMesh> Load(const std::string &id, const std::string& source) override {
        if (_resources.contains(id))
            return _resources[id];

        _logger->info("Loading mesh [{}] from '{}'", id, source);

        std::ifstream file(source);
        if (!file.is_open()) {
            _logger->error("Failed to load mesh [{}] from file '{}'", id, source);
            return nullptr;
        }

        std::vector<Vertex> vertices;
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#')
                continue;

            std::stringstream ss(line);

            float px, py, pz;
            float u, v;

            if (!(ss >> px >> py >> pz >> u >> v)) {
                _logger->error("Failed to load mesh [{}]: Invalid mesh format in: {}", id, line);
                return nullptr;
            }

            vertices.push_back({
                {px, py, pz},
                {u, v}
            });
        }

        MeshDesc desc{
            .vertices = vertices,
            .indexes = {}
        };
        std::shared_ptr<render::IMesh> mesh;
        std::string errorMessage;

        if (!_fabric->TryCreateMesh(desc, mesh, errorMessage)) {
            _logger->error(errorMessage);
            return nullptr;
       }

        _logger->debug("Loaded mesh [{}] successfully", id);

        _resources.insert({id, mesh});
        return mesh;
    }

    void Unload(const std::string &id) override {
        if (!_resources.contains(id))
            return;

        _logger->info("Unloading mesh [{}]", id);

        auto resource = _resources.at(id);

        _resources.erase(id);
    }
};

}
