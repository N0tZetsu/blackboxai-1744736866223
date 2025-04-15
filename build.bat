@echo off
REM Build script for compiling CS2 cheat DLL using MinGW

REM Set path to your MinGW bin directory if not in PATH
REM set PATH=C:\MinGW\bin;%PATH%

REM Set Vulkan SDK path
set VULKAN_SDK=C:\Users\Admin\VulkanSDK

echo Building CS2 cheat DLL...

REM Compile all source files with debug info
g++ -c main.cpp aimbot.cpp esp.cpp menu.cpp memory.cpp csgo_cheat.cpp -I. -I"%VULKAN_SDK%\Include" -std=c++17 -g

REM Link object files into DLL with all required libraries
g++ -shared -o cs2_cheat.dll main.o aimbot.o esp.o menu.o memory.o csgo_cheat.o ^
    -L"%VULKAN_SDK%\Lib" -lvulkan-1 -lgdi32 -luser32 -lkernel32 ^
    -std=c++17 -DUNICODE -D_UNICODE

REM Clean up object files
del *.o

echo Build complete: cs2_cheat.dll

REM Instructions
echo.
echo El cheat ha sido compilado. Para usarlo:
echo 1. Inyectar cs2_cheat.dll en CS2
echo 2. Usar la tecla Insert para mostrar/ocultar el menu
echo 3. Configurar Aimbot y ESP desde el menu
echo.
pause
