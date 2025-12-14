#pragma once
#include "engine/render/command_buffer.h"
#include "engine/render/igraphic_factory.h"

#include "engine/render/backends/opengl/opengl_canvas.h"
#include "engine/render/backends/opengl/opengl_factory.h"
#include "engine/render/backends/opengl/opengl_render.h"

#include "engine/resources/assetsdb.h"
#include "engine/resources/mesh_pipe.h"
#include "engine/resources/shader_pipe.h"
#include "engine/resources/texture_pipe.h"

#include "engine/audio/audio_system.h"
#include "engine/audio/audio_events_manager.h"

#include <boost/di/extension/injector.hpp>

#include "loop.h"
#include "window_system.h"
#include "event_bus.h"
#include "input_system.h"

namespace engine {

template <typename GameT>
    requires std::derived_from<GameT, IGame>
class Engine final {
    std::shared_ptr<spdlog::logger> _logger;

    std::shared_ptr<WindowSystem> _windowSystem;
    std::shared_ptr<render::ICanvas> _canvas;
    std::shared_ptr<Loop> _loop;
    std::shared_ptr<ui::UICanvas> _uiCanvas;
    std::shared_ptr<AudioSystem> _audioSystem;
    std::shared_ptr<AudioEventsManager> _audioEventsManager;

    bool _initialized = false;
public:
    ~Engine() {
        Dispose();
    }

    bool Init() {
        auto injector = di::make_injector(
            di::bind<ApplicationState>().in(di::singleton),
            di::bind<ui::UICanvas>.in(di::singleton),
            di::bind<InputSystem>.in(di::singleton),
            di::bind<AudioSystem>.in(di::singleton),
            di::bind<AudioEventsManager>.in(di::singleton),
            di::bind<EventBus>.in(di::singleton),
            di::bind<Time>.in(di::singleton),
            di::bind<AssetsDb>.in(di::singleton),
            di::bind<Logger>.in(di::singleton),
            di::bind<IGame>.to<GameT>().in(di::singleton),
            di::bind<WindowSystem>.in(di::singleton),
            di::bind<Loop>.in(di::singleton),
            di::bind<render::IRenderBackend>.to<render::OpenGLRenderBackend>().in(di::singleton),
            di::bind<render::IGraphicFactory>.to<render::OpenGLFactory>().in(di::singleton),
            di::bind<render::ICanvas>.to<render::OpenGLCanvas>().in(di::singleton),
            di::bind<render::CommandBuffer>.in(di::singleton)
        );

        _logger = injector.template create<std::shared_ptr<Logger>>()->Get();
        _logger->info("Initializing engine...");

        _windowSystem = injector.template create<std::shared_ptr<WindowSystem>>();
        _canvas = injector.template create<std::shared_ptr<render::ICanvas>>();
        _loop = injector.template create<std::shared_ptr<Loop>>();
        _uiCanvas = injector.template create<std::shared_ptr<ui::UICanvas>>();
        _audioSystem = injector.template create<std::shared_ptr<AudioSystem>>();
        _audioEventsManager = injector.template create<std::shared_ptr<AudioEventsManager>>();

        const auto success =
            InitilizeSDL3() &&
            _windowSystem->Create("Ping Pong", {800, 600}) &&
            _canvas->Init() &&
            _uiCanvas->Init() &&
            _audioSystem->Init() &&
            _audioEventsManager->Init();

        if (!success) {
            _logger->error("Failed to initialize engine");
            return false;
        }

        _logger->info("Initialized engine successfully");
        _initialized = true;

        return true;
    }

    int Run() {
        const auto result = _loop->Run();
        Dispose();
        return result;
    }

    void Dispose() {
        if (!_initialized)
            return;

        _logger->debug("Disposing engine...");

        _audioEventsManager->Dispose();
        _audioSystem->Dispose();
        _canvas->Dispose();
        _windowSystem->Dispose();
        SDL_Quit();

        _logger->info("Disposed engine successfully");
        _initialized = false;
    }
private:
    bool InitilizeSDL3() {
        _logger->debug("Initializing sdl3...");
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            _logger->error("Failed to initialize SDL3: {}", SDL_GetError());
            return false;
        }
        _logger->info("Initialized sdl3 successfully");
        return true;
    }
};

}
