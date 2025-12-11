#pragma once

#include "engine/render/icanvas.h"
#include "engine/utils.h"
#include "igame.h"
#include "input_system.h"

#include "time.h"

namespace engine {

class Loop {
    using clock = std::chrono::high_resolution_clock;

    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<InputSystem> _inputSystem;
    std::shared_ptr<render::ICanvas> _canvas;
    std::shared_ptr<IGame> _game;
    std::shared_ptr<Time> _time;

    bool _running = true;
    SDL_Event _event{};
public:
    Loop(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<render::ICanvas>& canvas,
        const std::shared_ptr<IGame>& game,
        const std::shared_ptr<Time>& time,
        const std::shared_ptr<InputSystem>& inputSystem
    ) :
        _logger(logger->Get()),
        _inputSystem(inputSystem),
        _canvas(canvas),
        _game(game),
        _time(time) {
    }

    int Run() {
        _logger->info("Starting application main loop...");

        _game->OnStart();

        auto lastTime = clock::now();

        while (_running) {
            auto now = clock::now();
            std::chrono::duration<float> dt = now - lastTime;
            lastTime = now;

            float deltaTime = dt.count();
            deltaTime = std::clamp(deltaTime, 0.f, 0.3f);

            _time->deltaTime = deltaTime;

            PollEvents();

            _game->OnUpdate();
            _game->OnDraw();

            _canvas->Draw();
        }

        _game->OnQuit();

        _logger->info("Exited application main loop...");
        return EXIT_SUCCESS;
    }

    void Quit() {
        _logger->info("Quit");
        _running = false;
    }
private:
    void PollEvents() {
        while (SDL_PollEvent(&_event)) {
            if (_event.type == SDL_EVENT_QUIT)
                _running = false;

            _inputSystem->ProcessEvent(_event);
        }
    }
};

}