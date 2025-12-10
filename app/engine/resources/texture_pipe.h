#pragma once

#include <stb_image.h>

#include "texture_desc.h"
#include "resource_pipe.h"
#include "../render/igraphic_fabric.h"
#include "../render/itexture.h"
#include "spdlog/fmt/bundled/std.h"

namespace engine {

    class TexturePipe final : public ResourcePipe<render::ITexture> {
        std::shared_ptr<spdlog::logger> _logger;
        std::shared_ptr<render::IGraphicFabric> _fabric;
    public:
        explicit TexturePipe(
            const std::shared_ptr<Logger>& logger,
            const std::shared_ptr<render::IGraphicFabric>& fabric
        ) :
            _logger(logger->Get()),
            _fabric(fabric) {

        }

        std::shared_ptr<render::ITexture> Load(const std::string& path, const bool cache) override {
            if (_cache.contains(path))
                return _cache[path];

            _logger->info("Loading texture from '{}'", path);

            int w, h, channels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 4);
            if (!data) {
                _logger->error("Failed to load texture from {}", path);
                return nullptr;
            }

            const TextureDesc desc{
                .width = w,
                .height = h,
                .channels = channels,
                .data = data
            };
            std::shared_ptr<render::ITexture> texture;
            std::string errorMessage;

            if (!_fabric->TryCreateTexture(desc, texture, errorMessage)) {
                _logger->error(errorMessage);
                return nullptr;
            }

            stbi_image_free(data);

            _logger->debug("Loaded texture successfully from '{}'", path);

            if (cache)
                _cache.insert({path, texture});
            return texture;
        }

        void Unload(const std::string& path) override {
            if (!_cache.contains(path))
                return;

            _logger->info("Unloading texture from '{}'", path);

            auto resource = _cache.at(path);
            resource.reset();

            _cache.erase(path);
        }
    };

}
