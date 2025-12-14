#pragma once

#include "engine/render/icanvas.h"
#include "engine/core/window_system.h"
#include "engine/render/command_buffer.h"

namespace engine::render {

class OpenGLCanvas final : public ICanvas {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<WindowSystem> _windowSystem;
    std::shared_ptr<CommandBuffer> _commandBuffer;

    SDL_GLContext _context = nullptr;
    bool _initialized = false;
public:
    OpenGLCanvas(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<WindowSystem>& windowSystem,
        const std::shared_ptr<CommandBuffer>& commandBuffer
    );

    bool Init() override;
    void Draw() override;
    void Dispose() override;
};

}