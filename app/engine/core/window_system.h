#pragma once
#include "../utils.h"

namespace engine {

class WindowSystem final {
    std::shared_ptr<spdlog::logger> _logger;
    SDL_Window* _window = nullptr;

    bool _initialized = false;
public:
    explicit WindowSystem(
        const std::shared_ptr<Logger>& logger
    ) :
        _logger(logger->Get()) {
    }

    ~WindowSystem() {
        Dispose();
    }

    bool Create(const std::string& title, const glm::ivec2& size) {
        _logger->info("Start creating window...");

        _window = SDL_CreateWindow(title.c_str(), size.x, size.y, SDL_WINDOW_OPENGL);
        if (!_window) {
            _logger->error("Failed to create window: {}", SDL_GetError());
            return false;
        }

        _logger->debug("Created window");

        _initialized = true;
        return true;
    }

    void Dispose() {
        if (!_initialized)
            return;

        _logger->debug("Disposing window system...");

        SDL_DestroyWindow(_window);
        _initialized = false;

        _logger->info("Disposed window successfully");

    }

    SDL_Window* GetWindow() const {
        return _window;
    }
};

}
