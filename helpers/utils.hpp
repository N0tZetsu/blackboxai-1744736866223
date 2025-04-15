#pragma once
#include <Windows.h>
#include <string>

namespace Utils {
    void ConsolePrint(const char* fmt, ...);
    void ConsoleReadKey();
    void AttachConsole();
    void DetachConsole();
    
    // Memory utilities
    uintptr_t FindPattern(const char* module_name, const char* pattern);
    uintptr_t GetModuleBase(const char* module_name);
}
