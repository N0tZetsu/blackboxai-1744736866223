@echo off
REM Build script for compiling csgo_cheat.cpp into a DLL using MinGW

REM Set path to your MinGW bin directory if not in PATH
REM set PATH=C:\MinGW\bin;%PATH%

g++ -shared -o csgo_cheat.dll csgo_cheat.cpp -ld3d9 -ld3dx9 -limm32 -lgdi32 -luser32 -lkernel32 -std=c++17 -DUNICODE -D_UNICODE -I./imgui -I./imgui/backends

echo Build complete: csgo_cheat.dll
pause
