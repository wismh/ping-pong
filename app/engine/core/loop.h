#pragma once

#include "canvas.h"

namespace engine {

class Loop {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<Canvas> _canvas;

    bool _running = true;
    SDL_Event _event{};
public:
    Loop(
        const std::shared_ptr<spdlog::logger>& logger,
        const std::shared_ptr<Canvas>& canvas
    ) :
        _logger(logger),
        _canvas(canvas) {

    }

    int Run() {
        _logger->info("Starting application main loop...");

        while (_running) {
            PollEvent();
            _canvas->Draw();
        }

        _logger->info("Exited application main loop...");
        return EXIT_SUCCESS;
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