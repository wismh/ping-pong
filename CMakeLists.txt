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
add_subdirectory(app)