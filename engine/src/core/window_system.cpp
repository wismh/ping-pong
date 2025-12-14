#include "engine/core/window_system.h"

namespace engine {

WindowSystem::WindowSystem(
    const std::shared_ptr<Logger>& logger
) :
    _logger(logger->Get()) {
}

WindowSystem::~WindowSystem() {
    Dispose();
}

bool WindowSystem::Create(const std::string& title, const glm::ivec2& size) {
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

void WindowSystem::Dispose() {
    if (!_initialized)
        return;

    _logger->debug("Disposing window system...");

    SDL_DestroyWindow(_window);
    _initialized = false;

    _logger->info("Disposed window successfully");

}

SDL_Window* WindowSystem::GetWindow() const {
    return _window;
}

glm::ivec2 WindowSystem::Size() const {
    int width, height;
    SDL_GetWindowSize(_window, &width, &height);
    return glm::ivec2(width, height);
}

}