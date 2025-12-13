cmake_minimum_required(VERSION 3.15)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/SDL3)
add_subdirectory(external/glm)
add_subdirectory(external/glad)
add_subdirectory(external/spdlog)
add_subdirectory(external/tinyxml2)

add_library(nanovg
    external/nanovg/src/nanovg.c
)

target_include_directories(nanovg PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

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
    app/engine/resources/resource_pipe.h
    app/engine/core/igame.h
    app/engine/resources/mesh_desc.h
    app/engine/resources/mesh_pipe.h
    app/engine/render/igpu_mesh.h
    app/engine/render/ishader.h
    app/engine/render/backends/opengl/opengl_mesh.h
    app/engine/resources/shader_desc.h
    app/engine/render/backends/opengl/opengl_shader.h
    app/engine/resources/resource_create_exception.h
    app/engine/render/backends/opengl/opengl_fabric.h
    app/engine/render/camera.h
    app/engine/render/camera.cpp
    app/engine/render/itexture.h
    app/engine/render/backends/opengl/opengl_texture.h
    app/engine/resources/texture_desc.h
    app/engine/resources/texture_pipe.h
    app/engine/resources/shader_pipe.h
    app/engine/nodes/node.h
    app/engine/ecs/ecs.h
    app/engine/nodes/node_ecs.h
    app/game/game.h
    app/game/utils.h
    app/engine/ecs/render_system.h
    app/engine/ecs/renderable.h
    app/engine/ecs/transform.h
    app/engine/resources/assetsdb.h
    app/game/game_scene.h
    app/game/ball_system.h
    app/game/ball.h
    app/engine/core/time.h
    app/game/player.h
    app/engine/ecs/colliders.h
    app/engine/core/event_bus.h
    app/engine/ecs/physcis_system.h
    app/engine/ecs/rigidbody.h
    app/engine/core/input_system.h
    app/game/input_actions.h
    app/game/player_controll_system.h
    app/engine/nodes/node_ui.h
    app/engine/ui/ui_element.h
    app/engine/ui/label.h
    app/engine/ui/ui_canvas.h
    app/engine/ui/layout.h
    app/engine/ui/ui_canvas.cpp
    app/game/game_state.h
    app/game/game_presenter_system.h
    app/game/game_ui_system.h
    app/game/player_ai_system.h
    app/engine/resources/ui_image.h
    app/engine/resources/ui_image_pipe.h
        app/engine/core/application_state.h
)
# ---------- Definitions ----------
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

# ---------- Includes ----------
target_include_directories(${PROJECT_NAME} PRIVATE
    external/boost_di/include
    external/boost_di/extension/include
    external/nanovg/src
)

# ---------- Link everything ----------
target_link_libraries(${PROJECT_NAME}
    SDL3::SDL3
    glad
    glm
    spdlog::spdlog_header_only
    nanovg
    tinyxml2
)