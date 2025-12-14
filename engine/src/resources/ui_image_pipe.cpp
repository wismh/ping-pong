#include "engine/resources/ui_image_pipe.h"

namespace engine {

UIImagePipe::UIImagePipe(
    const std::shared_ptr<Logger>& logger,
    const std::shared_ptr<ui::UICanvas>& canvas
) :
    _logger(logger->Get()),
    _canvas(canvas) {

}

std::shared_ptr<UIImage> UIImagePipe::Load(const std::string& path, const bool cache) {
    if (_cache.contains(path))
        return _cache[path];

    _logger->info("Loading ui image from '{}'", path);

    auto image = std::make_shared<UIImage>();
    image->image = nvgCreateImage(_canvas->GetContext(), path.c_str(), 0);
    if (image->image == 0) {
        _logger->error("Failed load ui image from '{}'", path);
        return nullptr;
    }

    _logger->debug("Loaded ui image successfully from '{}'", path);

    if (cache)
        _cache.insert({path, image});
    return image;
}

void UIImagePipe::Unload(const std::string& path) {
    if (!_cache.contains(path))
        return;

    _logger->info("Unloading ui image '{}'", path);

    auto resource = _cache.at(path);
    resource.reset();

    _cache.erase(path);
}

}