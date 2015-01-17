#!/bin/bash

cd build
#cmake -G"Unix Makefiles" ..
#make VERBOSE=1
/home/developer/emscripten/emscripten/master/emcmake cmake ..
/home/developer/emscripten/emscripten/master/emmake make VERBOSE=1

cd ..
