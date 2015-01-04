cmake_minimum_required(VERSION 2.8)

# Settings
set(EMSCRIPTEN_PREFIX "/home/developer/emscripten/emscripten/master")
set(EMSCRIPTEN_SDL2 "home/developer/MuchFireSoHot/SDL-emscripten/")

set(CMAKE_C_COMPILER "${EMSCRIPTEN_PREFIX}/emcc")
set(CMAKE_CXX_COMPILER "${EMSCRIPTEN_PREFIX}/em++")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -s USE_GLFW=3 -Wno-warn-absolute-paths")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -s USE_GLFW=3 -Wno-warn-absolute-paths")

set(CMAKE_TOOLCHAIN_FILE "/home/developer/emscripten/emscripten/master/cmake/Modules/Platform/Emscripten.cmake")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Source
file(GLOB_RECURSE SOURCE_FILES
    "include/*.h"
    "src/*.cpp"
)

# Project settings
project(MuchFireSoHot)
include_directories("${EMSCRIPTEN_PREFIX}/system/include" "include" "3rdparty/glm")
add_definitions(-D__EMSCRIPTEN__)
add_executable(MuchFireSoHot ${SOURCE_FILES})
