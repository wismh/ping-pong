#include "ui_canvas.h"

#include "glad/glad.h"

#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

namespace engine::ui {

bool UICanvas::Init() {
    _logger->info("Initializing UICanvas");

    _vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    if (!_vg) {
        _logger->error("Failed to init NanoVG");
        return false;
    }


    _logger->debug("Initialized UICanvas successfully");
    return true;
}

void UICanvas::Draw(const std::shared_ptr<UIElement> &element) {
    const glm::vec2 size = {static_cast<float>(_window->Size().x), static_cast<float>(_window->Size().y)};

    nvgBeginFrame(_vg, size.x, size.y, size.x / size.y);

    element->Draw(_vg, {}, size);

    nvgEndFrame(_vg);
}

NVGcontext *UICanvas::GetContext() {
    return _vg;
}

} // namespace engine::ui
