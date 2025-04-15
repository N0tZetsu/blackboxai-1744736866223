@echo off
REM Build script for compiling CS2 cheat DLL using MinGW

REM Set path to your MinGW bin directory if not in PATH
REM set PATH=C:\MinGW\bin;%PATH%

REM Set Vulkan SDK path - Update this to match your Vulkan SDK installation
set VULKAN_SDK=C:\VulkanSDK\1.3.250.1

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
echo Before running the cheat:
echo 1. Install Vulkan SDK from https://vulkan.lunarg.com/
echo 2. Update VULKAN_SDK path in this script to match your installation
echo 3. Inject cs2_cheat.dll into CS2 using your preferred injector
echo.
pause
