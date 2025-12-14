#pragma once

#include "engine/ui/ui_canvas.h"

#include "ui_image.h"
#include "resource_pipe.h"

namespace engine {

class UIImagePipe final : public ResourcePipe<UIImage> {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<ui::UICanvas> _canvas;
public:
    explicit UIImagePipe(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<ui::UICanvas>& canvas
    );

    std::shared_ptr<UIImage> Load(const std::string& path, bool cache) override;
    void Unload(const std::string& path) override;
};

}
