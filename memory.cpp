#include "memory.h"
#include <iostream>

bool Memory::Initialize() {
    // Get CS2 process ID
    DWORD processId = 0;
    HWND window = FindWindowA("SDL_app", "Counter-Strike 2"); // Updated for CS2
    if (!window) {
        std::cerr << "Failed to find CS2 window\n";
        return false;
    }
    GetWindowThreadProcessId(window, &processId);
    if (!processId) {
        std::cerr << "Failed to get process ID\n";
        return false;
    }

    // Open process handle
    handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
    if (!handle) {
        std::cerr << "Failed to open process\n";
        return false;
    }

    // Get module addresses - CS2 uses different module names
    if (!GetModuleBaseAddress(L"client.dll", clientModule) ||
        !GetModuleBaseAddress(L"engine2.dll", engineModule)) {
        std::cerr << "Failed to get module addresses\n";
        Cleanup();
        return false;
    }

    return true;
}

void Memory::Cleanup() {
    if (handle) {
        CloseHandle(handle);
        handle = nullptr;
    }
    clientModule = 0;
    engineModule = 0;
}

bool Memory::GetModuleBaseAddress(const wchar_t* moduleName, uintptr_t& baseAddress) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetProcessId(handle));
    if (snapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    MODULEENTRY32W moduleEntry;
    moduleEntry.dwSize = sizeof(moduleEntry);

    if (Module32FirstW(snapshot, &moduleEntry)) {
        do {
            if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
                baseAddress = (uintptr_t)moduleEntry.modBaseAddr;
                CloseHandle(snapshot);
                return true;
            }
        } while (Module32NextW(snapshot, &moduleEntry));
    }

    CloseHandle(snapshot);
    return false;
}

// CS2-specific helper functions
uintptr_t Memory::GetLocalPlayerPawn() {
    // In CS2, we need to get the controller first, then the pawn
    uintptr_t localController = Read<uintptr_t>(clientModule + Offsets::dwLocalPlayerController);
    if (!localController) return 0;

    // Get pawn handle from controller
    uintptr_t pawnHandle = Read<uintptr_t>(localController + Offsets::m_hPlayerPawn);
    if (!pawnHandle) return 0;

    // Get actual pawn pointer from entity list
    return GetEntityPawnByIndex(pawnHandle & 0x7FFF);
}

uintptr_t Memory::GetEntityPawnByIndex(int index) {
    // CS2 uses a different entity list structure
    uintptr_t entityList = Read<uintptr_t>(clientModule + Offsets::dwEntityList);
    if (!entityList) return 0;

    // Get list entry
    uintptr_t listEntry = Read<uintptr_t>(entityList + (8 * (index >> 9) + 16));
    if (!listEntry) return 0;

    // Get entity pointer
    return Read<uintptr_t>(listEntry + 120 * (index & 0x1FF));
}

Matrix4x4 Memory::GetViewMatrix() {
    // Read view matrix from new CS2 location
    return Read<Matrix4x4>(clientModule + Offsets::dwViewMatrix);
}
