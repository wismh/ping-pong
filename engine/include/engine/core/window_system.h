#pragma once
#include "engine/utils.h"

namespace engine {

class WindowSystem final {
    std::shared_ptr<spdlog::logger> _logger;
    SDL_Window* _window = nullptr;

    bool _initialized = false;
public:
    explicit WindowSystem(
        const std::shared_ptr<Logger>& logger
    );
    ~WindowSystem();

    bool Create(const std::string& title, const glm::ivec2& size);
    void Dispose();

    SDL_Window* GetWindow() const;
    glm::ivec2 Size() const;
};

}
