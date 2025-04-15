@echo off
REM Build script for compiling CS:GO cheat DLL using MinGW

REM Set path to your MinGW bin directory if not in PATH
REM set PATH=C:\MinGW\bin;%PATH%

echo Building CS:GO cheat DLL...

REM Compile all source files
g++ -c main.cpp aimbot.cpp esp.cpp menu.cpp -I. -std=c++17

REM Link object files into DLL
g++ -shared -o csgo_cheat.dll main.o aimbot.o esp.o menu.o -ld3d9 -limm32 -lgdi32 -luser32 -lkernel32 -std=c++17 -DUNICODE -D_UNICODE

REM Clean up object files
del *.o

echo Build complete: csgo_cheat.dll
pause
