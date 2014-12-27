@echo off
Rem DO NOT SUBMIT CHANGES

set emc_path="C:\Program Files\Emscripten\emscripten\1.25.0"

cd build
cmake -DCMAKE_TOOLCHAIN_FILE="%EMSCRIPTEN%/cmake/Modules/Platform/emscripten.cmake" -G "Unix Makefiles" ..
cd ..
