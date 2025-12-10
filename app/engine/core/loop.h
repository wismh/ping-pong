#pragma once

#include "igame.h"
#include "engine/render/icanvas.h"
#include "engine/utils.h"

namespace engine {

class Loop {
    using clock = std::chrono::high_resolution_clock;

    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<render::ICanvas> _canvas;
    std::shared_ptr<IGame> _game;

    bool _running = true;
    SDL_Event _event{};
public:
    Loop(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<render::ICanvas>& canvas,
        const std::shared_ptr<IGame>& game
    ) :
        _logger(logger->Get()),
        _canvas(canvas),
        _game(game) {
    }

    int Run() {
        _logger->info("Starting application main loop...");

        _game->OnStart();

        auto lastTime = clock::now();

        while (_running) {
            auto now = clock::now();
            std::chrono::duration<float> dt = now - lastTime;
            lastTime = now;

            const float deltaTime = dt.count();

            PollEvent();

            _game->OnUpdate(deltaTime);
            _game->OnDraw(deltaTime);

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
    void PollEvent() {
        while (SDL_PollEvent(&_event)) {
            if (_event.type == SDL_EVENT_QUIT)
                _running = false;
        }
    }
};

}