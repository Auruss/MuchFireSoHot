cmake_minimum_required(VERSION 2.8)

# Settings
set(EMSCRIPTEN_PREFIX "/home/developer/emscripten/emscripten/master")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

if("${EMSCRIPTEN}" STREQUAL "1")
    set(CMAKE_EXE_LINKER_FLAGS "-s USE_GLFW=3 -g4 -o MuchFireSoHot.js")
else()
    #set(CMAKE_EXE_LINKER_FLAGS "-s USE_GLFW=3 -g4 -o MuchFireSoHot.js")
endif()

# Source
file(GLOB_RECURSE SOURCE_FILES
    "include/*.h"
    "src/*.cpp"
)

# Project settings
project(MuchFireSoHot)

if("${EMSCRIPTEN}" STREQUAL "1")
    include_directories("${EMSCRIPTEN_PREFIX}/system/inlude" "include" "3rdparty/glm")
else()
    include_directories("include" "3rdparty/glm" "C:/Open Source/glfw-3.0.4/include" "C:/OpenGL/glew-1.10.0/include" "C:/Program Files/Emscripten/emscripten/1.25.0/system/include")
endif()

add_executable(MuchFireSoHot ${SOURCE_FILES})

if("${EMSCRIPTEN}" STREQUAL "1")
    add_definitions(-D__EMSCRIPTEN__ -std=c++11 -Wno-warn-absolute-paths -s USE_GLFW=3 -g4)
else()
    add_definitions(-D__EMSCRIPTEN__ -std=c++11 -Wno-warn-absolute-paths)
endif()