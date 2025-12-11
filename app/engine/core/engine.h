#pragma once

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "engine/render/command_buffer.h"
#include "engine/render/igraphic_fabric.h"

#include "engine/render/backends/opengl/opengl_canvas.h"
#include "engine/render/backends/opengl/opengl_render.h"
#include "engine/render/backends/opengl/opengl_fabric.h"

#include "window_system.h"
#include "loop.h"
#include "../resources/assetsdb.h"
#include "../resources/mesh_pipe.h"
#include "../resources/shader_pipe.h"
#include "../resources/texture_pipe.h"

#include <boost/di/extension/injector.hpp>

namespace engine {

template <typename GameT>
    requires std::derived_from<GameT, IGame>
class Engine final {
    std::shared_ptr<spdlog::logger> _logger;

    std::shared_ptr<WindowSystem> _windowSystem;
    std::shared_ptr<render::ICanvas> _canvas;
    std::shared_ptr<Loop> _loop;
public:
    ~Engine() {
        Dispose();
    }

    bool Init() {
        auto injector = di::make_injector(
            di::bind<Time>.in(di::singleton),
            di::bind<AssetsDb>.in(di::singleton),
            di::bind<Logger>.in(di::singleton),
            di::bind<IGame>.to<GameT>().in(di::singleton),
            di::bind<WindowSystem>.in(di::singleton),
            di::bind<Loop>.in(di::singleton),
            di::bind<render::IRenderBackend>.to<render::OpenGLRenderBackend>().in(di::singleton),
            di::bind<render::IGraphicFabric>.to<render::OpenGLFabric>().in(di::singleton),
            di::bind<render::ICanvas>.to<render::OpenGLCanvas>().in(di::singleton),
            di::bind<render::CommandBuffer>.in(di::singleton)
        );

        _logger = injector.template create<std::shared_ptr<Logger>>()->Get();
        _logger->info("Initializing engine...");

        _windowSystem = injector.template create<std::shared_ptr<WindowSystem>>();
        _canvas = injector.template create<std::shared_ptr<render::ICanvas>>();
        _loop = injector.template create<std::shared_ptr<Loop>>();

        const auto success =
            InitilizeSDL3() &&
            _windowSystem->Create("Ping Pong", {800, 600}) &&
            _canvas->Init();

        if (!success) {
            _logger->error("Failed to initialize engine");
            return false;
        }

        _logger->info("Initialized engine successfully");
        return true;
    }

    int Run() {
        const auto result = _loop->Run();
        Dispose();
        return result;
    }

    void Dispose() {
        _logger->debug("Disposing engine...");

        _canvas->Dispose();
        _windowSystem->Dispose();
        SDL_Quit();

        _logger->info("Disposed engine successfully");
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
