cmake_minimum_required(VERSION 3.15)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(nanovg STATIC external/nanovg/src/nanovg.c)
target_include_directories(nanovg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(external/SDL3)
add_subdirectory(external/glm)
add_subdirectory(external/glad)
add_subdirectory(external/spdlog)
add_subdirectory(external/tinyxml2)
add_subdirectory(external/SDL_mixer)

add_subdirectory(engine)

# ---------- Our Source ----------
add_executable(${PROJECT_NAME}
    app/main.cpp
    app/game/game.h
    app/game/utils.h
    app/game/game_scene.h
    app/game/ball_system.h
    app/game/ball.h
    app/game/player.h
    app/game/input_actions.h
    app/game/player_controll_system.h
    app/game/game_state.h
    app/game/game_presenter_system.h
    app/game/game_ui_system.h
    app/game/player_ai_system.h
    app/game/ai_params.h
)

target_link_libraries(${PROJECT_NAME} engine)
target_include_directories(${PROJECT_NAME} PRIVATE engine/include)