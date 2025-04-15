# CS:GO Advanced Cheat DLL

This project is an advanced cheat DLL for CS:GO featuring:

- Aimbot with FOV and smooth aiming options
- ESP with FOV options
- Menu similar to the provided screenshot using ImGui
- Menu toggled by the Insert key
- Compilable to DLL for injection with Exloader

## Project Structure

- `main.cpp` - DLL entry point and DirectX hooking
- `aimbot.h/cpp` - Aimbot implementation
- `esp.h/cpp` - ESP features
- `menu.h/cpp` - ImGui-based menu system
- `build.bat` - Build script for MinGW

## Requirements

- Windows OS
- MinGW-w64 with g++ compiler
- ImGui library

## Setup Instructions

1. **Download and Install MinGW-w64:**
   - Download from: https://winlibs.com/ or https://www.mingw-w64.org/
   - Add MinGW's bin directory to your system PATH

2. **Download ImGui:**
   - Get ImGui from [https://github.com/ocornut/imgui](https://github.com/ocornut/imgui)
   - Copy these files to your project directory:
     ```
     imgui.h
     imgui.cpp
     imgui_internal.h
     imgui_impl_dx9.h
     imgui_impl_dx9.cpp
     imgui_impl_win32.h
     imgui_impl_win32.cpp
     imgui_draw.cpp
     imgui_widgets.cpp
     imgui_tables.cpp
     ```

3. **Build the DLL:**
   - Open command prompt in the project directory
   - Run `build.bat`
   - The script will compile all source files and create `csgo_cheat.dll`

4. **Usage:**
   - Inject the compiled DLL into CS:GO using Exloader
   - Press Insert to toggle the cheat menu
   - Adjust aimbot and ESP settings in the menu

## Features

### Aimbot
- Configurable FOV
- Smooth aiming
- Target selection based on distance and FOV

### ESP
- Player ESP with configurable FOV
- Customizable ESP features

### Menu
- ImGui-based user interface
- Toggle with Insert key
- Real-time configuration of all features

## Notes

- This cheat is for educational purposes only
- Using cheats in online games is unethical and may result in bans
- Some features require implementation of game-specific memory reading and writing
