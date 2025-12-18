#pragma once

#include <SDL3/SDL.h>
#include <boost/di.hpp>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <spdlog/logger.h>
#include <iostream>
#include <string>
#include <variant>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

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

    const std::shared_ptr<spdlog::logger>& Get() {
        return _logger;
    }
};

}