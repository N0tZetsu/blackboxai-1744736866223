#pragma once
#include <Windows.h>

void InitializeMenu(HWND window);
void RenderMenu();
void ToggleMenu();
bool IsMenuVisible();
void CleanupMenu();

// Menu settings getters/setters
void SetMenuPosition(int x, int y);
