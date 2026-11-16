#pragma once
#include <windows.h>
#include "Snake.h"
#include "Food.h"
#include "Menu.h"
#include "Settings.h"
#include "SpeedManager.h"

class Game {
public:
    static const int CLIENT_WIDTH = 800;
    static const int CLIENT_HEIGHT = 600;
    static const int GRID_CELL_SIZE = 20;
    static const int COLS = CLIENT_WIDTH / GRID_CELL_SIZE;   // 40
    static const int ROWS = CLIENT_HEIGHT / GRID_CELL_SIZE;  // 30

    Game();
    ~Game();

    // Start or restart a game session
    void RestartGame(HWND hwnd);

    // Run game logic loop (ticks on window timer)
    void Tick(HWND hwnd);

    // Process paint message (flicker-free GDI double-buffer)
    void Paint(HWND hwnd, HDC hdc);

    // Process keyboard inputs
    void HandleInput(HWND hwnd, WPARAM wParam);
    void HandleKeyDown(HWND hwnd, WPARAM wParam, bool wasDown);
    void HandleKeyUp(HWND hwnd, WPARAM wParam);

    // Get current timer interval based on difficulty and score progression
    int GetTimerInterval() const;

private:
    Snake snake;
    Food food;
    Menu menu;
    Settings settings;
    SpeedManager speedManager;

    int score = 0;
    int currentInterval = 100; // in milliseconds
    bool isFirstStart = true;

    // Helper functions for drawing
    void DrawGrid(HDC hdc, int w, int h);
    void DrawSnake(HDC hdc);
    void DrawFood(HDC hdc);
    void DrawHUD(HDC hdc, int w, int h);

    // Sound effect player helpers
    void PlayEatSound();
    void PlayDeathSound();

    // Manage Win32 window timer according to progressive speeds
    void UpdateTimer(HWND hwnd);
};
