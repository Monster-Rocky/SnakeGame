# Classic Snake Game

A lightweight, standalone, highly-optimized **Classic Snake Game** built using native **C++17** and the **Win32 API/GDI** for Windows 10/11 (64-bit).

This project features:
* **Extremely Small Binary size** (~250 KB)
* **Under 1-second startup time**
* **Very low RAM/CPU footprint** (runs well under 10MB RAM)
* **Flicker-Free smooth graphics** (using GDI double-buffering)
* **Zero external dependencies** (runs fully offline, no heavy game engines or frameworks)
* **Retro synthesizer beep sounds** (procedurally generated on asynchronous threads)
* **Persistent settings & High Scores** (using a simple local `.ini` configuration)

---

## Controls

* **Navigate Menus**: `UP`/`DOWN` arrow keys or `W`/`S` keys. `ENTER` to select.
* **Movement**: Arrow keys or `W`/`A`/`S`/`D` keys.
* **Pause / Resume**: `P` key.
* **Temporary Speed Boost**: Hold the `SPACEBAR` key during play to temporarily boost the snake's speed. Release to return to standard speed.
* **Back to Menu**: `ESC` key (saves high scores and configurations).
* **Start / Restart Game**: `ENTER` key.

---

## Game Features

* **Grid Size**: 800x600 resolution mapped to a 40x30 coordinate system.
* **Custom Customization**: Toggle Grid ON/OFF and Sound ON/OFF dynamically in the settings.
* **Difficulty Modes**: Choose from Easy (starting speed 150ms), Normal (100ms), or Hard (65ms).
* **Hybrid Speed Control System**: 
  - **AUTOMATIC**: The speed dynamically transitions from `SLOW` (180ms) to `MEDIUM` (110ms) to `FAST` (70ms) and gradually increases further as your score climbs.
  - **MANUAL**: Choose a permanent base speed of `LOW` (150ms), `MEDIUM` (100ms), or `HIGH` (60ms).
  - **SPACEBAR BOOST**: Pressing and holding the spacebar temporary boosts the snake to `BOOST` speed (35ms).
* **High Score Tracking**: Stores and tracks high scores locally across sessions.

---

## Project Structure

```text
SnakeGame/
├── src/
│   ├── main.cpp        # WinMain entry point, WndProc event loop
│   ├── Game.h/.cpp     # Core double-buffered GDI loop, collision engine, and audio
│   ├── Snake.h/.cpp    # Deque coordinate system representing body segments
│   ├── Food.h/.cpp     # Food spawn random generator (collision checked)
│   ├── Menu.h/.cpp     # Render layouts for Main, Settings, Paused, and Game Over states
│   ├── Settings.h/.cpp # Key-value INI persistence configuration
│   └── SpeedManager.h/.cpp # Hybrid Speed calculations, states, limits, and spacebar boost
├── resources/
│   ├── resource.rc     # Resource definition binding the application icon
│   └── snake.ico       # Retro compiled green snake head application icon
├── CMakeLists.txt      # Build configuration (Win32 target, LTCG/LTO optimizations)
├── build.bat           # Executable compilation automation script
└── README.md           # Documentation
```
