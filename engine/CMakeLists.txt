cmake_minimum_required(VERSION 3.15)
project(engine CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDLMIXER_OGG OFF CACHE BOOL "" FORCE)
set(SDLMIXER_OGG_STB OFF CACHE BOOL "" FORCE)
set(SDLMIXER_OGG_VORBIS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_WAV ON CACHE BOOL "" FORCE)
set(SDLMIXER_VENDORED OFF CACHE BOOL "" FORCE)

add_library(${PROJECT_NAME} STATIC
    include/engine/core/engine.h
    include/engine/core/window_system.h
    include/engine/utils.h
    include/engine/core/loop.h
    include/engine/render/command_buffer.h
    include/engine/render/commands.h
    include/engine/render/irender_backend.h
    include/engine/render/icanvas.h
    include/engine/render/igraphic_fabric.h
    include/engine/render/backends/opengl/opengl_canvas.h
    include/engine/render/backends/opengl/opengl_render.h
    include/engine/resources/resource_pipe.h
    include/engine/core/igame.h
    include/engine/resources/mesh_desc.h
    include/engine/resources/mesh_pipe.h
    include/engine/render/igpu_mesh.h
    include/engine/render/ishader.h
    include/engine/render/backends/opengl/opengl_mesh.h
    include/engine/resources/shader_desc.h
    include/engine/render/backends/opengl/opengl_shader.h
    include/engine/resources/resource_create_exception.h
    include/engine/render/backends/opengl/opengl_fabric.h
    include/engine/render/camera.h
    include/engine/render/itexture.h
    include/engine/render/backends/opengl/opengl_texture.h
    include/engine/resources/texture_desc.h
    include/engine/resources/texture_pipe.h
    include/engine/resources/shader_pipe.h
    include/engine/nodes/node.h
    include/engine/ecs/ecs.h
    include/engine/nodes/node_ecs.h
    include/engine/ecs/render_system.h
    include/engine/ecs/renderable.h
    include/engine/ecs/transform.h
    include/engine/resources/assetsdb.h
    include/engine/core/time.h
    include/engine/ecs/colliders.h
    include/engine/core/event_bus.h
    include/engine/ecs/physcis_system.h
    include/engine/ecs/rigidbody.h
    include/engine/core/input_system.h
    include/engine/nodes/node_ui.h
    include/engine/ui/ui_element.h
    include/engine/ui/label.h
    include/engine/ui/ui_canvas.h
    include/engine/ui/layout.h
    include/engine/ui/ui_canvas.cpp
    include/engine/resources/ui_image.h
    include/engine/resources/ui_image_pipe.h
    include/engine/core/application_state.h
    include/engine/audio/audio_system.h
    include/engine/audio/audio.h
    include/engine/audio/audio_events_manager.h

    src/render/command_buffer.cpp
    src/render/backend/opengl/opengl_canvas.cpp
    src/render/backend/opengl/opengl_fabric.cpp
    src/render/backend/opengl/opengl_mesh.cpp
    src/render/backend/opengl/opengl_render.cpp
    src/render/backend/opengl/opengl_shader.cpp
    src/render/backend/opengl/opengl_texture.cpp
    src/render/camera.cpp
    src/core/input_system.cpp
    src/core/loop.cpp
    src/core/window_system.cpp
    src/ecs/physics_system.cpp
    src/ecs/render_system.cpp
    src/node/node.cpp
    src/node/node_ecs.cpp
    src/node/node_ui.cpp
    src/resources/audio_pipe.cpp
    src/resources/mesh_pipe.cpp
    src/resources/shader_pipe.cpp
    src/resources/texture_pipe.cpp
    src/resources/ui_image_pipe.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ../external/boost_di/include
    ../external/boost_di/extension/include
    ../external/nanovg/src
    ./include/
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets"
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3
    SDL3_mixer::SDL3_mixer
    glad
    glm
    spdlog::spdlog_header_only
    nanovg
    tinyxml2
)