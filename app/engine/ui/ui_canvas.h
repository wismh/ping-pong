#pragma once
#include "../core/window_system.h"
#include "ui_element.h"

namespace engine::ui {

class UICanvas final {
    NVGcontext* _vg = nullptr;
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<WindowSystem> _window;
public:
    explicit UICanvas(const std::shared_ptr<Logger> &logger, const std::shared_ptr<WindowSystem> &window) :
        _logger(logger->Get()), _window(window) {}

    bool Init();
    void Draw(const std::shared_ptr<UIElement>& element);
    NVGcontext* GetContext() {
        return _vg;
    }
};

}