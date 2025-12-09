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

namespace engine {

class Engine final {
    std::shared_ptr<spdlog::logger> _logger;

    std::shared_ptr<WindowSystem> _windowSystem;
    std::shared_ptr<render::ICanvas> _canvas;
    std::shared_ptr<Loop> _loop;
public:
    Engine() = default;
    ~Engine() {
        Dispose();
    }

    bool Init() {
        CreateLogger();
        _logger->info("Initializing engine...");

        auto injector = di::make_injector(
            di::bind<spdlog::logger>().to(_logger),
            di::bind<WindowSystem>.in(di::singleton),
            di::bind<Loop>.in(di::singleton),
            di::bind<render::IRenderBackend>.to<render::OpenGLRenderBackend>().in(di::singleton),
            di::bind<render::IGraphicFabric>.to<render::OpenGLFabric>().in(di::singleton),
            di::bind<render::CommandBuffer>.in(di::singleton),
            di::bind<render::ICanvas>.to<render::OpenGLCanvas>().in(di::singleton)
        );

        _windowSystem = injector.create<std::shared_ptr<WindowSystem>>();
        _canvas = injector.create<std::shared_ptr<render::ICanvas>>();
        _loop = injector.create<std::shared_ptr<Loop>>();

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

    void CreateLogger() {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("game.log", true);

        _logger = std::make_shared<spdlog::logger>(
            "engine",
            spdlog::sinks_init_list{console_sink, file_sink}
        );

        _logger->set_level(spdlog::level::trace);
        _logger->flush_on(spdlog::level::info);
    }
};

}
