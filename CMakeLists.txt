cmake_minimum_required(VERSION 3.15)
project(SnakeGame LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Game.h
    src/Snake.cpp
    src/Snake.h
    src/Food.cpp
    src/Food.h
    src/Menu.cpp
    src/Menu.h
    src/Settings.cpp
    src/Settings.h
    src/SpeedManager.cpp
    src/SpeedManager.h
    resources/resource.rc
)

# Executable configured with WIN32 to suppress console window
add_executable(SnakeGame WIN32 ${SOURCES})

# Compile and link options for minimal size and high performance
if(MSVC)
    target_compile_options(SnakeGame PRIVATE /O2 /Os /GL /Gw /Gy)
    target_link_options(SnakeGame PRIVATE /LTCG /OPT:REF /OPT:ICF)
else()
    # For GCC and Clang (LLVM MinGW)
    target_compile_options(SnakeGame PRIVATE -O3 -Os -flto -ffunction-sections -fdata-sections)
    target_link_options(SnakeGame PRIVATE -static -flto -s -Wl,--gc-sections -mwindows)
endif()

# Link against native Windows MultiMedia library for audio support
target_link_libraries(SnakeGame PRIVATE winmm)
