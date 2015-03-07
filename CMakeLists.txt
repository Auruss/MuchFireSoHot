cmake_minimum_required(VERSION 2.8)

# Settings
if(WIN32)
    set(EMSCRIPTEN_PREFIX "C:/emscripten")
else()
    set(EMSCRIPTEN_PREFIX "/home/developer/emscripten/emscripten/master")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Globals
if(WIN32)
    SET(COMMON_CMDS "--cache C:/.emscripten_cache --ports C:/.emscripten_ports -s NO_EXIT_RUNTIME=1")
else()
    SET(COMMON_CMDS "")
endif()

if("${EMSCRIPTEN}" STREQUAL "1")
    set(CMAKE_EXE_LINKER_FLAGS "--preload-file ../data/compiled/ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s STB_IMAGE=1 -s USE_GLFW=3 --tracing -g4 -o MuchFireSoHot.js -s EXPORTED_FUNCTIONS=\"['_editor_update_vals','_main','_real_main']\" ${COMMON_CMDS}")
else()
    
endif()

# Source
file(GLOB_RECURSE SOURCE_FILES
    "include/*.h"
    "src/*.cpp"
)

# Project settings
project(MuchFireSoHot)

if("${EMSCRIPTEN}" STREQUAL "1")
    include_directories("${EMSCRIPTEN_PREFIX}/system/include" "include" "3rdparty/glm" "3rdparty/boost")
else()
    include_directories("include" "3rdparty/glm" "3rdparty/boost" "C:/Open Source/glfw-3.0.4/include" "C:/OpenGL/glew-1.10.0/include" "C:/Program Files/Emscripten/emscripten/1.25.0/system/include")
endif()

add_executable(MuchFireSoHot ${SOURCE_FILES})

if("${EMSCRIPTEN}" STREQUAL "1")
    add_definitions(-D__EMSCRIPTEN__ --tracing -std=c++11 -Wno-warn-absolute-paths -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s STB_IMAGE=1 -s USE_GLFW=3 -g4 -s EXPORTED_FUNCTIONS=\"['_editor_update_vals','_main','_real_main']\")
    add_definitions(${COMMON_CMDS})
else()
    add_definitions(-D__EMSCRIPTEN__ -std=c++11 -Wno-warn-absolute-paths -D__GXX_EXPERIMENTAL_CXX0X__)
endif()