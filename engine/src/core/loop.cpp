#include "engine/core/loop.h"

namespace engine {

Loop::Loop(
    const std::shared_ptr<Logger>& logger,
    const std::shared_ptr<render::ICanvas>& canvas,
    const std::shared_ptr<IGame>& game,
    const std::shared_ptr<Time>& time,
    const std::shared_ptr<InputSystem>& inputSystem,
    const std::shared_ptr<ApplicationState>& appState
) :
    _logger(logger->Get()),
    _inputSystem(inputSystem),
    _canvas(canvas),
    _game(game),
    _time(time),
    _appState(appState) {
}

int Loop::Run() {
    _logger->info("Starting application main loop...");

    _game->OnStart();

    auto lastTime = clock::now();

    while (_appState->_running) {
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

void Loop::PollEvents() {
    while (SDL_PollEvent(&_event)) {
        if (_event.type == SDL_EVENT_QUIT)
            _appState->Quit();

        _inputSystem->ProcessEvent(_event);
    }
}

}