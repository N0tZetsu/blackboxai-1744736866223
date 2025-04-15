#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <cstdint>

// CS2 specific offsets (Source 2 engine)
namespace Offsets {
    // Client.dll offsets
    constexpr auto dwLocalPlayerController = 0x1810F48;    // Updated for CS2
    constexpr auto dwEntityList = 0x17C1950;              // Updated for CS2
    constexpr auto dwViewMatrix = 0x1820150;              // Updated for CS2
    constexpr auto dwGlobalVars = 0x169C0D0;             // New in CS2
    
    // Entity offsets
    constexpr auto m_hPlayerPawn = 0x7EC;                 // New in CS2
    constexpr auto m_iHealth = 0x32C;                     // Updated for CS2
    constexpr auto m_iTeamNum = 0x3BF;                    // Updated for CS2
    constexpr auto m_vecOrigin = 0x1224;                  // Updated for CS2
    constexpr auto m_vecViewOffset = 0x108;               // Updated for CS2
    constexpr auto m_dwBoneMatrix = 0x1124;               // Updated for CS2
    constexpr auto m_bSpottedByMask = 0x1698;            // Updated for CS2
}

// CS2 specific structures
struct Vector3 {
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
};

struct Matrix4x4 {
    float m[4][4];
};

class Memory {
public:
    static Memory& Get() {
        static Memory instance;
        return instance;
    }

    bool Initialize();
    void Cleanup();

    // Memory reading utilities
    template<typename T>
    T Read(uintptr_t address) {
        T buffer;
        ReadProcessMemory(handle, (LPCVOID)address, &buffer, sizeof(T), nullptr);
        return buffer;
    }

    template<typename T>
    void Write(uintptr_t address, T value) {
        WriteProcessMemory(handle, (LPVOID)address, &value, sizeof(T), nullptr);
    }

    // CS2 specific functions
    uintptr_t GetClientModule() const { return clientModule; }
    uintptr_t GetEngineModule() const { return engineModule; }
    HANDLE GetProcessHandle() const { return handle; }

    // New CS2 helper functions
    uintptr_t GetLocalPlayerPawn();
    uintptr_t GetEntityPawnByIndex(int index);
    Matrix4x4 GetViewMatrix();

private:
    Memory() : handle(nullptr), clientModule(0), engineModule(0) {}
    ~Memory() { Cleanup(); }
    Memory(const Memory&) = delete;
    Memory& operator=(const Memory&) = delete;

    bool GetModuleBaseAddress(const wchar_t* moduleName, uintptr_t& baseAddress);

    HANDLE handle;
    uintptr_t clientModule;
    uintptr_t engineModule;
};

// CS2 player structure
struct Player {
    bool isEnemy;
    bool isAlive;
    int health;
    Vector3 position;
    Vector3 headPosition;
    int teamNum;
    uintptr_t controller;  // New in CS2
    uintptr_t pawn;       // New in CS2
};
