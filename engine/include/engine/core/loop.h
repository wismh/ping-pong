#pragma once

#include "engine/render/icanvas.h"
#include "engine/utils.h"

#include "time.h"
#include "igame.h"
#include "input_system.h"
#include "application_state.h"

namespace engine {

class Loop {
    using clock = std::chrono::high_resolution_clock;

    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<InputSystem> _inputSystem;
    std::shared_ptr<render::ICanvas> _canvas;
    std::shared_ptr<IGame> _game;
    std::shared_ptr<Time> _time;
    std::shared_ptr<ApplicationState> _appState;

    SDL_Event _event{};
public:
    Loop(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<render::ICanvas>& canvas,
        const std::shared_ptr<IGame>& game,
        const std::shared_ptr<Time>& time,
        const std::shared_ptr<InputSystem>& inputSystem,
        const std::shared_ptr<ApplicationState>& appState
    );

    int Run();
private:
    void PollEvents();
};

}