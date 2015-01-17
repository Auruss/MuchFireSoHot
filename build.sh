#!/bin/bash

cd build

/home/developer/emscripten/emscripten/master/emcmake cmake ..
/home/developer/emscripten/emscripten/master/emmake make VERBOSE=1

cd ..