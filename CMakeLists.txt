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
        app/engine/core/utils.h
        app/engine/core/canvas.h
        app/engine/core/loop.h
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