@echo off

cd build
start /WAIT /B C:\emscripten\emcmake cmake .. -G "NMake Makefiles"
start /WAIT /B C:\emscripten\emmake nmake
cd ..
pause