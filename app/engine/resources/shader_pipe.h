#pragma once

#include <stb_image.h>

#include "shader_desc.h"
#include "resource_pipe.h"
#include "tinyxml2.h"
#include "../render/igraphic_fabric.h"
#include "../render/ishader.h"

namespace engine {

    class ShaderPipe final : public ResourcePipe<const std::string&, render::IShader> {
        std::shared_ptr<spdlog::logger> _logger;
        std::shared_ptr<render::IGraphicFabric> _fabric;
    public:
        explicit ShaderPipe(
            const std::shared_ptr<Logger>& logger,
            const std::shared_ptr<render::IGraphicFabric>& fabric
        ) :
            _logger(logger->Get()),
            _fabric(fabric) {

        }

        std::shared_ptr<render::IShader> Load(const std::string& id, const std::string& source) override {
            if (_resources.contains(id))
                return _resources[id];

            _logger->info("Loading shader [{}] from '{}'", id, source);

            using namespace tinyxml2;

            XMLDocument doc;

            if (doc.LoadFile(source.c_str()) != XML_SUCCESS) {
                _logger->error("Failed to load shader [{}]", id);
                return nullptr;
            }

            auto* root = doc.RootElement();
            if (!root) {
                _logger->error("Shader [{}] has no XML root element", id);
                return nullptr;
            }

            auto* v = root->FirstChildElement("vertex");
            if (!v || !v->GetText()) {
                _logger->error("Vertex shader missing in [{}]", id);
                return nullptr;
            }
            const auto& vertex = v->GetText();

            auto* f = root->FirstChildElement("fragment");
            if (!f || !f->GetText()) {
                _logger->error("Fragment shader missing in [{}]", id);
                return nullptr;
            }
            const auto& fragment = f->GetText();

            const ShaderDesc desc{
                .vertexShaderSrc = vertex,
                .fragmentShaderSrc = fragment
            };
            std::shared_ptr<render::IShader> shader;
            std::string errorMessage;

            if (!_fabric->TryCreateShader(desc, shader, errorMessage)) {
                _logger->error(errorMessage);
                return nullptr;
            }

            _logger->debug("Loaded shader [{}] successfully", id);

            _resources.insert({id, shader});
            return shader;
        }

        void Unload(const std::string &id) override {
            if (!_resources.contains(id))
                return;

            _logger->info("Unloading shader [{}]", id);

            auto resource = _resources.at(id);
            resource.reset();

            _resources.erase(id);
        }
    };

}
