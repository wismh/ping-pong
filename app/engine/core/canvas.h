#pragma once

#include "window_system.h"

namespace engine {

class Canvas {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<WindowSystem> _windowSystem;
    SDL_GLContext _context = nullptr;

    bool _initialized = false;
public:
    Canvas(
        const std::shared_ptr<spdlog::logger>& logger,
        const std::shared_ptr<WindowSystem>& windowSystem
    ) :
        _logger(logger),
        _windowSystem(windowSystem) {

    }

    bool Init() {
        _logger->debug("Initializing canvas...");

        _context = SDL_GL_CreateContext(_windowSystem->GetWindow());
        if (!_context) {
            _logger->error("Failed to create GL context: {}", SDL_GetError());
            return false;
        }

        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));

        _logger->info("Initialized canvas successfully");

        _initialized = true;
        return true;
    }

    void Draw() const {
        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(_windowSystem->GetWindow());
    }

    void Dispose() {
        if (!_initialized)
            return;

        SDL_GL_DestroyContext(_context);
        _initialized = false;
    }
};

}