#include "Game.h"
#include <thread>
#include <string>

Game::Game() {
    // Settings loaded automatically on startup.
    // The main menu is active by default.
    menu.state = MenuState::MainMenu;
}

Game::~Game() {
}

void Game::RestartGame(HWND hwnd) {
    snake.Reset(COLS / 2, ROWS / 2);
    food.Spawn(COLS, ROWS, snake);
    score = 0;

    // Initialize SpeedManager settings from player's profile config
    speedManager.mode = settings.speedModeAuto ? SpeedMode::Automatic : SpeedMode::Manual;
    speedManager.manualSpeed = static_cast<SpeedLevel>(settings.manualSpeedLevel);
    speedManager.ResetBoost();

    currentInterval = speedManager.CalculateCurrentSpeed(score);

    UpdateTimer(hwnd);
}

void Game::UpdateTimer(HWND hwnd) {
    KillTimer(hwnd, 1);
    SetTimer(hwnd, 1, currentInterval, NULL);
}

int Game::GetTimerInterval() const {
    return speedManager.CalculateCurrentSpeed(score);
}

void Game::Tick(HWND hwnd) {
    if (menu.state != MenuState::Playing) return;

    snake.Move();

    // Check collisions
    if (snake.CheckWallCollision(COLS, ROWS) || snake.CheckSelfCollision()) {
        PlayDeathSound();
        speedManager.ResetBoost(); // Clear boost state on death
        if (score > settings.highScore) {
            settings.highScore = score;
            settings.Save();
        }
        menu.state = MenuState::GameOver;
        InvalidateRect(hwnd, NULL, FALSE);
        return;
    }

    // Check food consumption
    if (!snake.body.empty() && snake.body.front() == food.pos) {
        PlayEatSound();
        snake.Grow();
        score += 10;
        food.Spawn(COLS, ROWS, snake);

        // Adjust progressive difficulty speed
        int targetInterval = GetTimerInterval();
        if (targetInterval != currentInterval) {
            currentInterval = targetInterval;
            UpdateTimer(hwnd);
        }
    }

    // Request redraw
    InvalidateRect(hwnd, NULL, FALSE);
}

void Game::Paint(HWND hwnd, HDC hdc) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Set up flicker-free double buffering
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    // Draw dark background
    HBRUSH bgBrush = CreateSolidBrush(RGB(18, 18, 18));
    FillRect(memDC, &rect, bgBrush);
    DeleteObject(bgBrush);

    if (menu.state == MenuState::Playing || menu.state == MenuState::Paused) {
        // Draw grid lines (if configured)
        if (settings.gridOn) {
            DrawGrid(memDC, width, height);
        }

        // Draw food
        DrawFood(memDC);

        // Draw snake
        DrawSnake(memDC);

        // Draw HUD details (Score, High Score)
        DrawHUD(memDC, width, height);

        // Overlays menu if paused
        if (menu.state == MenuState::Paused) {
            menu.Draw(memDC, width, height, settings, score);
        }
    } else {
        // Draw menu overlays for Main Menu, Settings, and Game Over
        menu.Draw(memDC, width, height, settings, score);
    }

    // Copy to screen
    BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

    // Release backbuffer objects
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
}

void Game::DrawGrid(HDC hdc, int w, int h) {
    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(30, 30, 30));
    HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);

    // Vertical grid lines
    for (int x = 0; x <= COLS; ++x) {
        MoveToEx(hdc, x * GRID_CELL_SIZE, 0, NULL);
        LineTo(hdc, x * GRID_CELL_SIZE, h);
    }

    // Horizontal grid lines
    for (int y = 0; y <= ROWS; ++y) {
        MoveToEx(hdc, 0, y * GRID_CELL_SIZE, NULL);
        LineTo(hdc, w, y * GRID_CELL_SIZE);
    }

    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);
}

void Game::DrawFood(HDC hdc) {
    RECT r = {
        food.pos.x * GRID_CELL_SIZE + 2,
        food.pos.y * GRID_CELL_SIZE + 2,
        (food.pos.x + 1) * GRID_CELL_SIZE - 2,
        (food.pos.y + 1) * GRID_CELL_SIZE - 2
    };

    // Draw food as a sleek red cherry/apple
    HBRUSH foodBrush = CreateSolidBrush(RGB(231, 76, 60));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, foodBrush);

    HPEN foodPen = CreatePen(PS_SOLID, 1, RGB(192, 57, 43));
    HPEN oldPen = (HPEN)SelectObject(hdc, foodPen);

    Ellipse(hdc, r.left, r.top, r.right, r.bottom);

    // Clean up
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(foodBrush);
    DeleteObject(foodPen);
}

void Game::DrawSnake(HDC hdc) {
    if (snake.body.empty()) return;

    // Draw snake body segments
    HBRUSH bodyBrush = CreateSolidBrush(RGB(39, 174, 96));
    HPEN bodyPen = CreatePen(PS_SOLID, 1, RGB(30, 132, 73));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, bodyBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, bodyPen);

    for (size_t i = 1; i < snake.body.size(); ++i) {
        RECT r = {
            snake.body[i].x * GRID_CELL_SIZE + 1,
            snake.body[i].y * GRID_CELL_SIZE + 1,
            (snake.body[i].x + 1) * GRID_CELL_SIZE - 1,
            (snake.body[i].y + 1) * GRID_CELL_SIZE - 1
        };
        RoundRect(hdc, r.left, r.top, r.right, r.bottom, 6, 6);
    }

    // Draw snake head
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(bodyBrush);
    DeleteObject(bodyPen);

    HBRUSH headBrush = CreateSolidBrush(RGB(46, 204, 113));
    HPEN headPen = CreatePen(PS_SOLID, 1, RGB(39, 174, 96));
    SelectObject(hdc, headBrush);
    SelectObject(hdc, headPen);

    Point head = snake.body.front();
    RECT hr = {
        head.x * GRID_CELL_SIZE + 1,
        head.y * GRID_CELL_SIZE + 1,
        (head.x + 1) * GRID_CELL_SIZE - 1,
        (head.y + 1) * GRID_CELL_SIZE - 1
    };
    RoundRect(hdc, hr.left, hr.top, hr.right, hr.bottom, 8, 8);

    // Draw eyes to make the game design premium
    HBRUSH eyeBrush = CreateSolidBrush(RGB(18, 18, 18));
    SelectObject(hdc, eyeBrush);

    int ex1 = 0, ey1 = 0, ex2 = 0, ey2 = 0;
    int size = 3;

    switch (snake.dir) {
        case Direction::Up:
            ex1 = hr.left + 5; ey1 = hr.top + 5;
            ex2 = hr.right - 8; ey2 = hr.top + 5;
            break;
        case Direction::Down:
            ex1 = hr.left + 5; ey1 = hr.bottom - 8;
            ex2 = hr.right - 8; ey2 = hr.bottom - 8;
            break;
        case Direction::Left:
            ex1 = hr.left + 5; ey1 = hr.top + 5;
            ex2 = hr.left + 5; ey2 = hr.bottom - 8;
            break;
        case Direction::Right:
            ex1 = hr.right - 8; ey1 = hr.top + 5;
            ex2 = hr.right - 8; ey2 = hr.bottom - 8;
            break;
    }
    
    // Draw left/top eye
    RECT rEye1 = { ex1, ey1, ex1 + size, ey1 + size };
    FillRect(hdc, &rEye1, eyeBrush);

    // Draw right/bottom eye
    RECT rEye2 = { ex2, ey2, ex2 + size, ey2 + size };
    FillRect(hdc, &rEye2, eyeBrush);

    // Clean up
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(headBrush);
    DeleteObject(headPen);
    DeleteObject(eyeBrush);
}

void Game::DrawHUD(HDC hdc, int w, int h) {
    // Create text font
    HFONT hHudFont = CreateFontW(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT oldFont = (HFONT)SelectObject(hdc, hHudFont);
    SetTextColor(hdc, RGB(236, 240, 241));
    SetBkMode(hdc, TRANSPARENT);

    // Write current score
    wchar_t scoreStr[32];
    wsprintfW(scoreStr, L"SCORE: %d", score);
    RECT rScore = { 15, 10, 200, 40 };
    DrawTextW(hdc, scoreStr, -1, &rScore, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

    // Write dynamic Speed Mode & Speed value in the center
    std::wstring modeStr = speedManager.mode == SpeedMode::Automatic ? L"AUTO" : L"MANUAL";
    std::wstring speedValStr = speedManager.GetSpeedHUDString(score);
    std::wstring hudStr = L"MODE: " + modeStr + L"   SPEED: " + speedValStr;
    RECT rHUD = { 200, 10, w - 200, 40 };
    DrawTextW(hdc, hudStr.c_str(), -1, &rHUD, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    // Write high score
    wchar_t bestStr[32];
    wsprintfW(bestStr, L"HIGH SCORE: %d", settings.highScore);
    RECT rBest = { w - 215, 10, w - 15, 40 };
    DrawTextW(hdc, bestStr, -1, &rBest, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

    SelectObject(hdc, oldFont);
    DeleteObject(hHudFont);
}

void Game::PlayEatSound() {
    if (settings.soundOn) {
        std::thread([]() {
            // Retro short beep sound for eating food
            Beep(650, 60);
        }).detach();
    }
}

void Game::PlayDeathSound() {
    if (settings.soundOn) {
        std::thread([]() {
            // Lower tone game over beep sound
            Beep(180, 300);
        }).detach();
    }
}

void Game::HandleInput(HWND hwnd, WPARAM wParam) {
    if (menu.state == MenuState::Playing) {
        switch (wParam) {
            case VK_UP:
            case 'W':
            case 'w':
                snake.ChangeDirection(Direction::Up);
                break;
            case VK_DOWN:
            case 'S':
            case 's':
                snake.ChangeDirection(Direction::Down);
                break;
            case VK_LEFT:
            case 'A':
            case 'a':
                snake.ChangeDirection(Direction::Left);
                break;
            case VK_RIGHT:
            case 'D':
            case 'd':
                snake.ChangeDirection(Direction::Right);
                break;
            case 'P':
            case 'p':
                if (settings.soundOn) {
                    std::thread([]() { Beep(400, 50); }).detach();
                }
                speedManager.ResetBoost();
                currentInterval = GetTimerInterval();
                UpdateTimer(hwnd);
                menu.state = MenuState::Paused;
                InvalidateRect(hwnd, NULL, FALSE);
                break;
            case VK_ESCAPE:
                // Handle ESC: pauses game and exits to main menu
                if (score > settings.highScore) {
                    settings.highScore = score;
                    settings.Save();
                }
                speedManager.ResetBoost();
                menu.state = MenuState::MainMenu;
                InvalidateRect(hwnd, NULL, FALSE);
                break;
        }
    }
    else if (menu.state == MenuState::Paused) {
        if (wParam == 'P' || wParam == 'p') {
            if (settings.soundOn) {
                std::thread([]() { Beep(500, 50); }).detach();
            }
            speedManager.ResetBoost();
            currentInterval = GetTimerInterval();
            UpdateTimer(hwnd);
            menu.state = MenuState::Playing;
            InvalidateRect(hwnd, NULL, FALSE);
        } else {
            bool shouldExit = false;
            bool shouldRestart = false;
            menu.HandleInput(wParam, settings, shouldExit, shouldRestart);
            
            if (shouldExit) {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            }
            InvalidateRect(hwnd, NULL, FALSE);
        }
    }
    else {
        // MainMenu, SettingsMenu, or GameOver
        bool shouldExit = false;
        bool shouldRestart = false;
        menu.HandleInput(wParam, settings, shouldExit, shouldRestart);

        if (shouldExit) {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        if (shouldRestart) {
            RestartGame(hwnd);
        }
        InvalidateRect(hwnd, NULL, FALSE);
    }
}

void Game::HandleKeyDown(HWND hwnd, WPARAM wParam, bool wasDown) {
    if (wParam == VK_SPACE) {
        // Spacebar Boost is only active while playing
        if (!wasDown && menu.state == MenuState::Playing) {
            speedManager.isBoostActive = true;
            int targetInterval = GetTimerInterval();
            if (targetInterval != currentInterval) {
                currentInterval = targetInterval;
                UpdateTimer(hwnd);
            }
            InvalidateRect(hwnd, NULL, FALSE);
        }
        return;
    }

    // Delegate all other key presses to standard controller
    HandleInput(hwnd, wParam);
}

void Game::HandleKeyUp(HWND hwnd, WPARAM wParam) {
    if (wParam == VK_SPACE) {
        if (speedManager.isBoostActive) {
            speedManager.isBoostActive = false;
            int targetInterval = GetTimerInterval();
            if (targetInterval != currentInterval) {
                currentInterval = targetInterval;
                UpdateTimer(hwnd);
            }
            InvalidateRect(hwnd, NULL, FALSE);
        }
    }
}
