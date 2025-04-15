@echo off
REM Build script for compiling CS2 cheat DLL using Visual Studio

REM Setup VS2019 environment (ajusta la ruta según tu versión de Visual Studio)
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"

echo Building CS2 cheat DLL...

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Compile all source files
cl /c /MD /I"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include" ^
    main.cpp ^
    aimbot.cpp ^
    esp.cpp ^
    menu.cpp ^
    memory.cpp ^
    csgo_cheat.cpp ^
    hooks.cpp ^
    options.cpp

REM Link to create DLL
link /DLL /OUT:build\cs2_cheat.dll ^
    /LIBPATH:"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86" ^
    main.obj ^
    aimbot.obj ^
    esp.obj ^
    menu.obj ^
    memory.obj ^
    csgo_cheat.obj ^
    hooks.obj ^
    options.obj ^
    d3d9.lib ^
    d3dx9.lib ^
    user32.lib ^
    kernel32.lib ^
    gdi32.lib

REM Clean up object files
del *.obj

echo.
echo Para compilar necesitas:
echo 1. Visual Studio 2019 (Community Edition es gratis)
echo    - Descargar de: https://visualstudio.microsoft.com/vs/older-downloads/
echo    - Durante la instalación, seleccionar "Desarrollo de escritorio con C++"
echo.
echo 2. DirectX SDK (June 2010)
echo    - Descargar de: https://www.microsoft.com/en-us/download/details.aspx?id=6812
echo.
echo 3. Después de instalar:
echo    - Ejecutar este archivo build.bat
echo    - El DLL se creará en la carpeta build
echo.
pause
