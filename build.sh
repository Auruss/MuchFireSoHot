#!/bin/bash

cd build
#cmake -G"Unix Makefiles" ..
#make VERBOSE=1
<<<<<<< HEAD
/home/developer/emscripten/emscripten/master/emcmake cmake ..
/home/developer/emscripten/emscripten/master/emmake make VERBOSE=1
=======
emcmake cmake ..
emmake make VERBOSE=1
>>>>>>> 9d78b702e3168e0430148e9e86ef6554a8b3ad13

cd ..
