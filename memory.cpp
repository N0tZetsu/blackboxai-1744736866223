#include "memory.h"
#include <iostream>

bool Memory::Initialize() {
    // Get CS:GO process ID
    DWORD processId = 0;
    HWND window = FindWindowA("Valve001", nullptr);
    if (!window) {
        std::cerr << "Failed to find CS:GO window\n";
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

    // Get module addresses
    if (!GetModuleBaseAddress(L"client.dll", clientModule) ||
        !GetModuleBaseAddress(L"engine.dll", engineModule)) {
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
