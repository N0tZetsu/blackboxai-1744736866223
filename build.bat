@echo off
REM Build script for compiling CS2 cheat DLL using MinGW

REM Set path to your MinGW bin directory if not in PATH
REM set PATH=C:\MinGW\bin;%PATH%

echo Building CS2 cheat DLL...

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Compile all source files with debug info
g++ -c main.cpp ^
    aimbot.cpp ^
    esp.cpp ^
    menu.cpp ^
    memory.cpp ^
    csgo_cheat.cpp ^
    -I. ^
    -I"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include" ^
    -std=c++17 -g

REM Link object files into DLL with all required libraries
g++ -shared -o build/cs2_cheat.dll *.o ^
    -L"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86" ^
    -ld3d9 -ld3dx9 -lgdi32 -luser32 -lkernel32 ^
    -std=c++17 -DUNICODE -D_UNICODE

REM Clean up object files
del *.o

echo Build complete: build/cs2_cheat.dll

REM Instructions
echo.
echo El cheat ha sido compilado en la carpeta 'build'.
echo Para usarlo:
echo 1. El archivo DLL esta en: build/cs2_cheat.dll
echo 2. Inyectar el DLL en CS2
echo 3. Usar la tecla Insert para mostrar/ocultar el menu
echo.
pause
