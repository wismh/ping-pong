#include <stb_image.h>

#include "engine/resources/texture_pipe.h"
#include "engine/resources/texture_desc.h"

namespace engine {

TexturePipe::TexturePipe(
    const std::shared_ptr<Logger>& logger,
    const std::shared_ptr<render::IGraphicFabric>& fabric
) :
    _logger(logger->Get()),
    _fabric(fabric) {

}

std::shared_ptr<render::ITexture> TexturePipe::Load(const std::string& path, const bool cache) {
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

void TexturePipe::Unload(const std::string& path) {
    if (!_cache.contains(path))
        return;

    _logger->info("Unloading texture from '{}'", path);

    auto resource = _cache.at(path);
    resource.reset();

    _cache.erase(path);
}

}