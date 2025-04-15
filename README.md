# CS:GO Advanced Cheat DLL

This project is an advanced cheat DLL for CS:GO featuring:

- Aimbot with FOV and smooth aiming options
- ESP with FOV options
- Menu similar to the provided screenshot using ImGui
- Menu toggled by the Insert key
- Compilable to DLL for injection with Exloader

---

## Features

- **Aimbot:** Smooth aiming with configurable FOV.
- **ESP:** Enemy highlighting with configurable FOV.
- **Menu:** ImGui-based menu for toggling features and adjusting settings.
- **Injection:** Designed to be injected as a DLL using Exloader or similar tools.

---

## Requirements

- Windows OS
- Visual Studio 2019 or later (or compatible C++ compiler)
- DirectX 9 SDK (for d3d9.lib and d3dx9.lib)
- ImGui library with DirectX9 and Win32 bindings

---

## Setup Instructions

1. **Clone or download this repository.**

2. **Download ImGui:**

   - Get ImGui from [https://github.com/ocornut/imgui](https://github.com/ocornut/imgui)
   - Copy the `imgui` folder into your project directory.
   - Include the DirectX9 and Win32 binding files from `imgui/backends`:
     - `imgui_impl_dx9.cpp`
     - `imgui_impl_dx9.h`
     - `imgui_impl_win32.cpp`
     - `imgui_impl_win32.h`

3. **Configure your Visual Studio project:**

   - Create a new DLL project.
   - Add `csgo_cheat.cpp` and ImGui source files.
   - Link against `d3d9.lib` and `d3dx9.lib`.
   - Set the subsystem to Windows.
   - Ensure the project is set to compile as a DLL.

4. **Build the project.**

5. **Inject the compiled DLL into CS:GO:**

   - Use Exloader or any DLL injector.
   - Toggle the cheat menu with the Insert key.

---

## Notes

- This cheat is for educational purposes only.
- Using cheats in online games is unethical and may result in bans.
- The code includes placeholders for game memory reading and hooking which must be implemented for full functionality.
- Further work is needed to implement actual game memory reading, hooking, and aimbot/ESP logic.

---

## License

This project is provided as-is without any warranty.
