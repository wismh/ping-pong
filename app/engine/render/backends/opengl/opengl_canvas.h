#pragma once

#include <iostream>

#include "../../commands.h"
#include "engine/render/icanvas.h"
#include "engine/core/window_system.h"

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
    ) :
        _logger(logger->Get()),
        _windowSystem(windowSystem),
        _commandBuffer(commandBuffer) {
    }

    bool Init() override {
        _logger->info("Initializing canvas...");

        _context = SDL_GL_CreateContext(_windowSystem->GetWindow());
        if (!_context) {
            _logger->error("Failed to create GL context: {}", SDL_GetError());
            return false;
        }

        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));

        _logger->info("OpenGL version info: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        _logger->debug("Initialized canvas successfully");

        _initialized = true;
        return true;
    }

    void Draw() override {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _commandBuffer->Execute();

        SDL_GL_SwapWindow(_windowSystem->GetWindow());
    }

    void Dispose() override {
        if (!_initialized)
            return;

        _logger->info("Disposing canvas...");

        SDL_GL_DestroyContext(_context);
        _initialized = false;

        _logger->debug("Disposed canvas successfully");
    }
};

}