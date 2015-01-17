#!/bin/bash

cd build
#cmake -G"Unix Makefiles" ..
#make VERBOSE=1
emcmake cmake ..
emmake make VERBOSE=1

cd ..
