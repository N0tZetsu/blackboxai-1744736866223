@echo off
REM Build script for compiling CS:GO cheat DLL using MinGW

REM Set path to your MinGW bin directory if not in PATH
REM set PATH=C:\MinGW\bin;%PATH%

echo Building CS:GO cheat DLL...

REM Create imgui directory if it doesn't exist
if not exist "imgui" mkdir imgui

echo Checking for ImGui files...
if not exist "imgui\imgui.h" (
    echo ERROR: ImGui files not found!
    echo Please copy the following files from ImGui to the imgui directory:
    echo   imgui.h
    echo   imgui.cpp
    echo   imgui_internal.h
    echo   imgui_impl_dx9.h
    echo   imgui_impl_dx9.cpp
    echo   imgui_impl_win32.h
    echo   imgui_impl_win32.cpp
    echo   imgui_draw.cpp
    echo   imgui_widgets.cpp
    echo   imgui_tables.cpp
    pause
    exit /b 1
)

REM Compile all source files
g++ -c main.cpp aimbot.cpp esp.cpp menu.cpp ^
    imgui/imgui.cpp ^
    imgui/imgui_impl_dx9.cpp ^
    imgui/imgui_impl_win32.cpp ^
    imgui/imgui_draw.cpp ^
    imgui/imgui_widgets.cpp ^
    imgui/imgui_tables.cpp ^
    -I. -Iimgui -std=c++17

REM Link object files into DLL
g++ -shared -o csgo_cheat.dll *.o -ld3d9 -limm32 -lgdi32 -luser32 -lkernel32 -std=c++17 -DUNICODE -D_UNICODE

REM Clean up object files
del *.o

echo Build complete: csgo_cheat.dll
pause
