cmake_minimum_required(VERSION 2.8)

# Settings
set(EMSCRIPTEN_PREFIX "/home/developer/emscripten/emscripten/master")

set(CMAKE_C_COMPILER "${EMSCRIPTEN_PREFIX}/emcc")
set(CMAKE_CXX_COMPILER "${EMSCRIPTEN_PREFIX}/em++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(CMAKE_TOOLCHAIN_FILE "/home/developer/emscripten/emscripten/master/cmake/Modules/Platform/Emscripten.cmake")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Source
file(GLOB SOURCE_FILES
    "include/*.h"
    "src/*.cpp"
)

# Project settings
project(MuchFireSoHot)
include_directories("${EMSCRIPTEN_PREFIX}/system/include" "include")
add_definitions(-D__EMSCRIPTEN__)
add_executable(MuchFireSoHot ${SOURCE_FILES})