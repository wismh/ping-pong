#pragma once
#include <fstream>

#include "mesh_desc.h"
#include "resource_pipe.h"
#include "../render/commands.h"

namespace engine {


class MeshPipe final : public ResourcePipe<render::IMesh> {
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

    std::shared_ptr<render::IMesh> Load(const std::string& path, const bool cache) override {
        if (_cache.contains(path))
            return _cache[path];

        _logger->info("Loading mesh from file '{}'", path);

        std::ifstream file(path);
        if (!file.is_open()) {
            _logger->error("Failed to load mesh from file '{}'", path);
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
                _logger->error("Failed to load mesh: Invalid mesh format in: {}",line);
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

        _logger->debug("Loaded mesh successfully from file: {}", path);

        if (cache)
            _cache.insert({path, mesh});

        return mesh;
    }

    void Unload(const std::string &source) override {
        if (!_cache.contains(source))
            return;

        _logger->info("Unloading mesh [{}]", source);

        auto resource = _cache.at(source);

        _cache.erase(source);
    }
};

}
