@echo off

cd build
start /WAIT /B C:\emscripten\emcmake cmake .. -G "NMake Makefiles"

set CL=/MP
start /WAIT /B C:\emscripten\emmake nmake

cd ..
pause