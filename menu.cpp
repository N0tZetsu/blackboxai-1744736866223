#include "menu.h"
#include "aimbot.h"
#include "esp.h"
#include <Windows.h>

namespace {
    bool showMenu = false;
    HWND gWindow = nullptr;
    HWND menuWindow = nullptr;
    int menuX = 100;
    int menuY = 100;

    // Window procedure for the menu window
    LRESULT CALLBACK MenuWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_CREATE: {
                // Create controls
                CreateWindowA("button", "Aimbot", 
                    WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                    10, 10, 100, 20, hwnd, (HMENU)1, nullptr, nullptr);

                CreateWindowA("button", "ESP", 
                    WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                    10, 40, 100, 20, hwnd, (HMENU)2, nullptr, nullptr);

                // Aimbot FOV slider
                CreateWindowA("static", "Aimbot FOV:", 
                    WS_VISIBLE | WS_CHILD,
                    10, 70, 100, 20, hwnd, nullptr, nullptr, nullptr);
                CreateWindowA("scrollbar", nullptr,
                    WS_VISIBLE | WS_CHILD | SBS_HORZ,
                    10, 90, 200, 20, hwnd, (HMENU)3, nullptr, nullptr);

                // ESP FOV slider
                CreateWindowA("static", "ESP FOV:", 
                    WS_VISIBLE | WS_CHILD,
                    10, 120, 100, 20, hwnd, nullptr, nullptr, nullptr);
                CreateWindowA("scrollbar", nullptr,
                    WS_VISIBLE | WS_CHILD | SBS_HORZ,
                    10, 140, 200, 20, hwnd, (HMENU)4, nullptr, nullptr);

                return 0;
            }

            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case 1: // Aimbot checkbox
                        SetAimbotEnabled(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED);
                        break;
                    case 2: // ESP checkbox
                        SetESPEnabled(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED);
                        break;
                }
                return 0;
            }

            case WM_HSCROLL: {
                HWND scrollbar = (HWND)lParam;
                int scrollID = GetDlgCtrlID(scrollbar);
                
                switch (scrollID) {
                    case 3: // Aimbot FOV slider
                        SetAimbotFOV((float)HIWORD(wParam));
                        break;
                    case 4: // ESP FOV slider
                        SetESPFOV((float)HIWORD(wParam));
                        break;
                }
                return 0;
            }

            case WM_CLOSE:
                ShowWindow(hwnd, SW_HIDE);
                showMenu = false;
                return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

void InitializeMenu(HWND window) {
    gWindow = window;

    // Register window class for menu
    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = MenuWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "CSGOCheatMenu";
    RegisterClassExA(&wc);

    // Create menu window
    menuWindow = CreateWindowExA(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        "CSGOCheatMenu",
        "Cheat Menu",
        WS_POPUP | WS_CAPTION | WS_SYSMENU,
        menuX, menuY, 250, 200,
        nullptr, nullptr,
        GetModuleHandle(NULL),
        nullptr
    );
}

void RenderMenu() {
    if (showMenu && !IsWindowVisible(menuWindow)) {
        ShowWindow(menuWindow, SW_SHOW);
    }
    else if (!showMenu && IsWindowVisible(menuWindow)) {
        ShowWindow(menuWindow, SW_HIDE);
    }
}

void ToggleMenu() {
    showMenu = !showMenu;
}

bool IsMenuVisible() {
    return showMenu;
}

void CleanupMenu() {
    if (menuWindow) {
        DestroyWindow(menuWindow);
        menuWindow = nullptr;
    }
    UnregisterClassA("CSGOCheatMenu", GetModuleHandle(NULL));
}

void SetMenuPosition(int x, int y) {
    menuX = x;
    menuY = y;
    if (menuWindow) {
        SetWindowPos(menuWindow, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
}
