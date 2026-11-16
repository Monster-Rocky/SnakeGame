#include "Menu.h"
#include <string>
#include <vector>

Menu::Menu() {
    CreateFonts();
}

Menu::~Menu() {
    CleanupFonts();
}

void Menu::CreateFonts() {
    // Create modern, high-quality typography fonts
    hTitleFont = CreateFontW(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

    hMenuFont = CreateFontW(24, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE,
                           ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

    hSubFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
}

void Menu::CleanupFonts() {
    if (hTitleFont) { DeleteObject(hTitleFont); hTitleFont = nullptr; }
    if (hMenuFont)  { DeleteObject(hMenuFont);  hMenuFont = nullptr;  }
    if (hSubFont)   { DeleteObject(hSubFont);   hSubFont = nullptr;   }
}

void Menu::Reset() {
    mainMenuSelection = 0;
    settingsSelection = 0;
}

void Menu::Draw(HDC hdc, int width, int height, const Settings& settings, int currentScore) {
    if (state == MenuState::Playing) {
        return; // The main game rendering is handled by the Game class
    }

    // Set text parameters for GDI
    SetBkMode(hdc, TRANSPARENT);
    
    // Draw background
    HBRUSH hBgBrush = CreateSolidBrush(RGB(18, 18, 18));
    RECT clientRect = { 0, 0, width, height };
    FillRect(hdc, &clientRect, hBgBrush);
    DeleteObject(hBgBrush);

    if (state == MenuState::MainMenu) {
        // Draw decorative retro borders
        HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(46, 204, 113));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
        
        // Inner border frame
        RECT borderRect = { 20, 20, width - 20, height - 20 };
        HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
        Rectangle(hdc, borderRect.left, borderRect.top, borderRect.right, borderRect.bottom);

        // Title
        SelectObject(hdc, hTitleFont);
        SetTextColor(hdc, RGB(46, 204, 113));
        RECT rTitle = { 0, 80, width, 180 };
        DrawTextW(hdc, L"CLASSIC SNAKE", -1, &rTitle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        // Subtitle
        SelectObject(hdc, hSubFont);
        SetTextColor(hdc, RGB(127, 140, 141));
        RECT rSub = { 0, 140, width, 180 };
        DrawTextW(hdc, L"A Native Win32 Retro Game", -1, &rSub, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        // Menu options
        std::wstring options[3] = { L"Play Game", L"Settings", L"Exit Game" };
        SelectObject(hdc, hMenuFont);

        for (int i = 0; i < 3; ++i) {
            RECT rOpt = { 0, 260 + i * 50, width, 300 + i * 50 };
            if (i == mainMenuSelection) {
                SetTextColor(hdc, RGB(46, 204, 113));
                std::wstring optText = L">  " + options[i] + L"  <";
                DrawTextW(hdc, optText.c_str(), -1, &rOpt, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            } else {
                SetTextColor(hdc, RGB(236, 240, 241));
                DrawTextW(hdc, options[i].c_str(), -1, &rOpt, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
        }

        // Footer instructions
        SelectObject(hdc, hSubFont);
        SetTextColor(hdc, RGB(127, 140, 141));
        RECT rFooter = { 0, height - 80, width, height - 30 };
        DrawTextW(hdc, L"Use UP/DOWN or W/S to navigate. ENTER to select.", -1, &rFooter, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        // Cleanup
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBorderPen);
    }
    else if (state == MenuState::SettingsMenu) {
        // Border
        HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(52, 152, 219));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
        RECT borderRect = { 20, 20, width - 20, height - 20 };
        HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
        Rectangle(hdc, borderRect.left, borderRect.top, borderRect.right, borderRect.bottom);

        // Title
        SelectObject(hdc, hTitleFont);
        SetTextColor(hdc, RGB(52, 152, 219));
        RECT rTitle = { 0, 80, width, 180 };
        DrawTextW(hdc, L"SETTINGS", -1, &rTitle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        // Menu options
        std::wstring soundVal = settings.soundOn ? L"ON" : L"OFF";
        std::wstring gridVal = settings.gridOn ? L"ON" : L"OFF";
        std::wstring diffVal = L"Normal";
        if (settings.difficulty == Difficulty::Easy) diffVal = L"Easy";
        else if (settings.difficulty == Difficulty::Hard) diffVal = L"Hard";

        std::vector<std::wstring> options;
        options.push_back(L"Sound:  < " + soundVal + L" >");
        options.push_back(L"Grid:  < " + gridVal + L" >");
        options.push_back(L"Difficulty:  < " + diffVal + L" >");
        options.push_back(L"Speed Mode:  < " + std::wstring(settings.speedModeAuto ? L"AUTOMATIC" : L"MANUAL") + L" >");
        
        if (!settings.speedModeAuto) {
            std::wstring speedVal = L"MEDIUM";
            if (settings.manualSpeedLevel == 0) speedVal = L"LOW";
            else if (settings.manualSpeedLevel == 2) speedVal = L"HIGH";
            options.push_back(L"Manual Speed:  < " + speedVal + L" >");
        }
        options.push_back(L"Back to Menu");

        SelectObject(hdc, hMenuFont);
        int maxOptions = static_cast<int>(options.size());
        for (int i = 0; i < maxOptions; ++i) {
            RECT rOpt = { 0, 200 + i * 45, width, 240 + i * 45 };
            if (i == settingsSelection) {
                SetTextColor(hdc, RGB(52, 152, 219));
                std::wstring optText = (i == maxOptions - 1) ? L"[ Back to Menu ]" : L"> " + options[i] + L" <";
                DrawTextW(hdc, optText.c_str(), -1, &rOpt, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            } else {
                SetTextColor(hdc, RGB(236, 240, 241));
                DrawTextW(hdc, options[i].c_str(), -1, &rOpt, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
        }

        // Footer instructions
        SelectObject(hdc, hSubFont);
        SetTextColor(hdc, RGB(127, 140, 141));
        RECT rFooter = { 0, height - 80, width, height - 30 };
        DrawTextW(hdc, L"Use UP/DOWN to move. LEFT/RIGHT or ENTER to change settings. ESC to save & exit.", -1, &rFooter, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        // Cleanup
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBorderPen);
    }
    else if (state == MenuState::Paused) {
        int boxW = 360;
        int boxH = 200;
        int boxX = (width - boxW) / 2;
        int boxY = (height - boxH) / 2;

        RECT rBox = { boxX, boxY, boxX + boxW, boxY + boxH };
        HBRUSH hBoxBrush = CreateSolidBrush(RGB(28, 28, 28));
        FillRect(hdc, &rBox, hBoxBrush);

        HPEN hBoxPen = CreatePen(PS_SOLID, 2, RGB(241, 196, 15)); // Gold accent for pause
        HPEN hOldPen = (HPEN)SelectObject(hdc, hBoxPen);
        HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
        
        Rectangle(hdc, rBox.left, rBox.top, rBox.right, rBox.bottom);
        Rectangle(hdc, rBox.left + 5, rBox.top + 5, rBox.right - 5, rBox.bottom - 5);

        // Draw text
        SelectObject(hdc, hTitleFont);
        SetTextColor(hdc, RGB(241, 196, 15));
        RECT rPaused = { boxX, boxY + 20, boxX + boxW, boxY + 80 };
        DrawTextW(hdc, L"PAUSED", -1, &rPaused, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        SelectObject(hdc, hSubFont);
        SetTextColor(hdc, RGB(236, 240, 241));
        RECT rResume = { boxX, boxY + 90, boxX + boxW, boxY + 120 };
        DrawTextW(hdc, L"Press 'P' to Resume", -1, &rResume, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        RECT rMenu = { boxX, boxY + 130, boxX + boxW, boxY + 160 };
        DrawTextW(hdc, L"Press 'ESC' to Quit to Menu", -1, &rMenu, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        // Cleanup
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBoxBrush);
        DeleteObject(hBoxPen);
    }
    else if (state == MenuState::GameOver) {
        HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(231, 76, 60));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
        RECT borderRect = { 20, 20, width - 20, height - 20 };
        HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
        Rectangle(hdc, borderRect.left, borderRect.top, borderRect.right, borderRect.bottom);

        // Title
        SelectObject(hdc, hTitleFont);
        SetTextColor(hdc, RGB(231, 76, 60));
        RECT rTitle = { 0, 80, width, 180 };
        DrawTextW(hdc, L"GAME OVER", -1, &rTitle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        // Score display
        SelectObject(hdc, hMenuFont);
        SetTextColor(hdc, RGB(236, 240, 241));
        
        wchar_t scoreText[64];
        wsprintfW(scoreText, L"Final Score: %d", currentScore);
        RECT rScore = { 0, 220, width, 260 };
        DrawTextW(hdc, scoreText, -1, &rScore, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        wchar_t bestText[64];
        wsprintfW(bestText, L"High Score: %d", settings.highScore);
        RECT rBest = { 0, 270, width, 310 };
        DrawTextW(hdc, bestText, -1, &rBest, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        // Check if new high score
        if (currentScore > 0 && currentScore >= settings.highScore) {
            SelectObject(hdc, hSubFont);
            SetTextColor(hdc, RGB(241, 196, 15)); // gold star color
            RECT rNewBest = { 0, 330, width, 360 };
            DrawTextW(hdc, L"★ NEW PERSONAL BEST! ★", -1, &rNewBest, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        }

        // Instructions
        SelectObject(hdc, hMenuFont);
        SetTextColor(hdc, RGB(46, 204, 113));
        RECT rRestart = { 0, 410, width, 450 };
        DrawTextW(hdc, L"Press ENTER to Restart", -1, &rRestart, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        SelectObject(hdc, hSubFont);
        SetTextColor(hdc, RGB(127, 140, 141));
        RECT rBack = { 0, 460, width, 490 };
        DrawTextW(hdc, L"Press ESC for Main Menu", -1, &rBack, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        // Cleanup
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBorderPen);
    }
}

void Menu::HandleInput(WPARAM wParam, Settings& settings, bool& shouldExit, bool& shouldRestartGame) {
    if (state == MenuState::MainMenu) {
        switch (wParam) {
            case VK_UP:
            case 'W':
            case 'w':
                mainMenuSelection = (mainMenuSelection - 1 + 3) % 3;
                break;
            case VK_DOWN:
            case 'S':
            case 's':
                mainMenuSelection = (mainMenuSelection + 1) % 3;
                break;
            case VK_RETURN:
                if (mainMenuSelection == 0) {
                    state = MenuState::Playing;
                    shouldRestartGame = true;
                } else if (mainMenuSelection == 1) {
                    state = MenuState::SettingsMenu;
                    settingsSelection = 0;
                } else if (mainMenuSelection == 2) {
                    shouldExit = true;
                }
                break;
        }
    }
    else if (state == MenuState::SettingsMenu) {
        int maxOptions = settings.speedModeAuto ? 5 : 6;
        switch (wParam) {
            case VK_UP:
            case 'W':
            case 'w':
                settingsSelection = (settingsSelection - 1 + maxOptions) % maxOptions;
                break;
            case VK_DOWN:
            case 'S':
            case 's':
                settingsSelection = (settingsSelection + 1) % maxOptions;
                break;
            case VK_LEFT:
            case 'A':
            case 'a':
                if (settingsSelection == 0) {
                    settings.soundOn = !settings.soundOn;
                } else if (settingsSelection == 1) {
                    settings.gridOn = !settings.gridOn;
                } else if (settingsSelection == 2) {
                    int diff = static_cast<int>(settings.difficulty);
                    diff = (diff - 1 + 3) % 3;
                    settings.difficulty = static_cast<Difficulty>(diff);
                } else if (settingsSelection == 3) {
                    settings.speedModeAuto = !settings.speedModeAuto;
                    int newMax = settings.speedModeAuto ? 5 : 6;
                    if (settingsSelection >= newMax) {
                        settingsSelection = newMax - 1;
                    }
                } else if (settingsSelection == 4 && !settings.speedModeAuto) {
                    settings.manualSpeedLevel = (settings.manualSpeedLevel - 1 + 3) % 3;
                }
                break;
            case VK_RIGHT:
            case 'D':
            case 'd':
            case VK_RETURN:
                if (settingsSelection == 0) {
                    settings.soundOn = !settings.soundOn;
                } else if (settingsSelection == 1) {
                    settings.gridOn = !settings.gridOn;
                } else if (settingsSelection == 2) {
                    int diff = static_cast<int>(settings.difficulty);
                    diff = (diff + 1) % 3;
                    settings.difficulty = static_cast<Difficulty>(diff);
                } else if (settingsSelection == 3) {
                    settings.speedModeAuto = !settings.speedModeAuto;
                    int newMax = settings.speedModeAuto ? 5 : 6;
                    if (settingsSelection >= newMax) {
                        settingsSelection = newMax - 1;
                    }
                } else if (settingsSelection == 4 && !settings.speedModeAuto) {
                    settings.manualSpeedLevel = (settings.manualSpeedLevel + 1) % 3;
                } else if (((settings.speedModeAuto && settingsSelection == 4) || (!settings.speedModeAuto && settingsSelection == 5)) && wParam == VK_RETURN) {
                    settings.Save();
                    state = MenuState::MainMenu;
                }
                break;
            case VK_ESCAPE:
                settings.Save();
                state = MenuState::MainMenu;
                break;
        }
    }
    else if (state == MenuState::Paused) {
        if (wParam == VK_ESCAPE) {
            state = MenuState::MainMenu;
        }
    }
    else if (state == MenuState::GameOver) {
        if (wParam == VK_RETURN) {
            state = MenuState::Playing;
            shouldRestartGame = true;
        } else if (wParam == VK_ESCAPE) {
            state = MenuState::MainMenu;
        }
    }
}
