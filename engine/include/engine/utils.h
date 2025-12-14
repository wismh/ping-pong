#pragma once

#include <../../../external/SDL3/include/SDL3/SDL.h>
#include <../../../external/boost_di/include/boost/di.hpp>
#include <../../../external/glad/include/glad/glad.h>
#include <../../../external/glm/glm/mat4x4.hpp>
#include <../../../external/glm/glm/vec2.hpp>
#include <../../../external/spdlog/include/spdlog/logger.h>
#include <iostream>
#include <string>
#include <variant>

#include "../../../external/spdlog/include/spdlog/sinks/basic_file_sink.h"
#include "../../../external/spdlog/include/spdlog/sinks/stdout_color_sinks.h"

namespace di = boost::di;

namespace engine {

class Logger final {
    std::shared_ptr<spdlog::logger> _logger;
public:
    Logger() {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("game.log", true);

        _logger = std::make_shared<spdlog::logger>(
            "engine",
            spdlog::sinks_init_list{console_sink, file_sink}
        );

        _logger->set_level(spdlog::level::trace);
        _logger->flush_on(spdlog::level::info);
    }

    std::shared_ptr<spdlog::logger> Get() {
        return _logger;
    }
};

}