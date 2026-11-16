#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "Game.h"

// Forward declaration of window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"SnakeGameWindowClass";

    WNDCLASSEXW wcex = {};
    wcex.cbSize        = sizeof(WNDCLASSEXW);
    wcex.lpfnWndProc   = WndProc;
    wcex.hInstance     = hInstance;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    // Load custom snake icon from resource compiled binary
    wcex.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCEW(101));
    wcex.hIconSm       = LoadIconW(hInstance, MAKEINTRESOURCEW(101));

    if (!RegisterClassExW(&wcex)) {
        MessageBoxW(NULL, L"Failed to register window class!", L"Error", MB_ICONERROR);
        return 1;
    }

    // Set up style and size (non-resizable window)
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    
    RECT r = { 0, 0, Game::CLIENT_WIDTH, Game::CLIENT_HEIGHT };
    AdjustWindowRect(&r, style, FALSE);
    
    int winWidth = r.right - r.left;
    int winHeight = r.bottom - r.top;

    // Center window on screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - winWidth) / 2;
    int posY = (screenHeight - winHeight) / 2;

    // Instantiate game state
    Game game;

    // Create the window
    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Classic Snake Game",
        style,
        posX, posY, winWidth, winHeight,
        NULL, NULL, hInstance, &game
    );

    if (hwnd == NULL) {
        MessageBoxW(NULL, L"Failed to create window!", L"Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Standard high-performance, low-CPU message loop
    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Game* game = reinterpret_cast<Game*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

    switch (msg) {
        case WM_NCCREATE: {
            CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreate->lpCreateParams));
            return TRUE;
        }
        case WM_TIMER: {
            if (game) {
                game->Tick(hwnd);
            }
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (game) {
                game->Paint(hwnd, hdc);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_KEYDOWN: {
            if (game) {
                bool wasDown = (lParam & (1 << 30)) != 0;
                game->HandleKeyDown(hwnd, wParam, wasDown);
            }
            return 0;
        }
        case WM_KEYUP: {
            if (game) {
                game->HandleKeyUp(hwnd, wParam);
            }
            return 0;
        }
        case WM_DESTROY: {
            // Cleanup timer
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}
