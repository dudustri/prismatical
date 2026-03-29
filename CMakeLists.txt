cmake_minimum_required(VERSION 3.28)
project(prismatical VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_COMPILER g++-14)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # use -std=c++23, not -std=gnu++23

# Collect all .cpp files under src/
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(prismatical ${SOURCES})

target_include_directories(prismatical PRIVATE src)

# Native Linux build — requires SDL2
if(NOT EMSCRIPTEN)
    find_package(SDL2 REQUIRED)
    target_link_libraries(prismatical PRIVATE SDL2::SDL2)
endif()

# WASM build — SDL2 is bundled in Emscripten, no find_package needed
if(EMSCRIPTEN)
    set_target_properties(prismatical PROPERTIES SUFFIX ".js")
    target_link_options(prismatical PRIVATE
        -sUSE_SDL=2
        -sUSE_WEBGL2=1
        -sALLOW_MEMORY_GROWTH=1
        --shell-file ${CMAKE_SOURCE_DIR}/web/shell.html
    )
endif()
