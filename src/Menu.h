#pragma once
#include <windows.h>
#include "Settings.h"

enum class MenuState {
    MainMenu,
    SettingsMenu,
    Playing,
    Paused,
    GameOver
};

class Menu {
public:
    MenuState state = MenuState::MainMenu;
    int mainMenuSelection = 0; // 0: Play, 1: Settings, 2: Exit
    int settingsSelection = 0; // 0: Sound, 1: Grid, 2: Difficulty, 3: Back

    Menu();
    ~Menu();

    // Reset menu selection indices
    void Reset();

    // Draw the active menu screen using GDI vector graphics and text
    void Draw(HDC hdc, int width, int height, const Settings& settings, int currentScore);

    // Navigate menus via keyboard input
    void HandleInput(WPARAM wParam, Settings& settings, bool& shouldExit, bool& shouldRestartGame);

private:
    HFONT hTitleFont = nullptr;
    HFONT hMenuFont = nullptr;
    HFONT hSubFont = nullptr;

    void CreateFonts();
    void CleanupFonts();
};
