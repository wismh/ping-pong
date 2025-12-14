cmake_minimum_required(VERSION 3.15)
project(game LANGUAGES CXX C)

add_executable(${PROJECT_NAME}
    include/game/game.h
    include/game/utils.h
    include/game/game_scene.h
    include/game/input_actions.h

    include/game/systems/ball_system.h
    include/game/systems/player_controll_system.h
    include/game/systems/game_presenter_system.h
    include/game/systems/game_ui_system.h
    include/game/systems/player_ai_system.h

    include/game/components/ball.h
    include/game/components/player.h
    include/game/components/game_state.h
    include/game/components/ai_params.h

    src/main.cpp
    src/game.cpp
    src/game_scene.cpp

    src/systems/ball_system.cpp
    src/systems/game_ui_system.cpp
    src/systems/player_ai_system.cpp
    src/systems/game_presenter_system.cpp
    src/systems/player_control_system.cpp
)

target_link_libraries(${PROJECT_NAME} engine)
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)