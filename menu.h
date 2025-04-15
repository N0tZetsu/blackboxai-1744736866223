#pragma once
#include <Windows.h>
#include <d3d9.h>

void InitializeMenu(HWND window, LPDIRECT3DDEVICE9 pDevice);
void RenderMenu();
void ToggleMenu();
bool IsMenuVisible();
void CleanupMenu();
