cmake_minimum_required(VERSION 3.15)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/SDL3)
add_subdirectory(external/glm)
add_subdirectory(external/glad)
add_subdirectory(external/spdlog)

# ---------- Our Source ----------
add_executable(${PROJECT_NAME}
    app/main.cpp
    app/engine/core/engine.h
    app/engine/core/window_system.h
    app/engine/utils.h
    app/engine/core/loop.h
    app/engine/render/command_buffer.h
    app/engine/render/commands.h
    app/engine/render/irender_backend.h
    app/engine/render/icanvas.h
    app/engine/render/igraphic_fabric.h
    app/engine/render/backends/opengl/opengl_canvas.h
    app/engine/render/backends/opengl/opengl_render.h
    app/engine/render/backends/opengl/opengl_fabric.h
)

# ---------- Includes ----------
target_include_directories(${PROJECT_NAME} PRIVATE
    external/boost_di/include
)

# ---------- Link everything ----------
target_link_libraries(${PROJECT_NAME}
    SDL3::SDL3
    glad
    glm
    spdlog::spdlog_header_only
)