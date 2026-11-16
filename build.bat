@echo off
setlocal

echo ===================================================
echo Building Classic Snake Game (Optimized Release Win64)
echo ===================================================

:: Setup temporary paths for CMake and LLVM MinGW compiler tools
set "CMAKE_PATH=C:\Program Files\CMake\bin"
set "COMPILER_PATH=C:\Users\Royal\AppData\Local\Microsoft\WinGet\Packages\MartinStorsjo.LLVM-MinGW.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\llvm-mingw-20260616-ucrt-x86_64\bin"

set "PATH=%CMAKE_PATH%;%COMPILER_PATH%;%PATH%"

:: Create build folder
if not exist "build" (
    mkdir build
)

echo.
echo [1/2] Configuring build files using CMake and LLVM MinGW...
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -B build -S .
if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: CMake configuration failed!
    exit /b %ERRORLEVEL%
)

echo.
echo [2/2] Compiling and linking SnakeGame.exe...
cmake --build build --config Release
if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: Compilation failed!
    exit /b %ERRORLEVEL%
)

echo.
echo ===================================================
echo Build completed successfully!
echo Executable is located at:
echo %~dp0build\SnakeGame.exe
echo ===================================================
pause
