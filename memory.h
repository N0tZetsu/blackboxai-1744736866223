#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <cstdint>

// CS:GO specific offsets
namespace Offsets {
    constexpr auto dwLocalPlayer = 0xDEA964;
    constexpr auto dwEntityList = 0x4DFFF14;
    constexpr auto dwViewMatrix = 0x4DF0D44;
    constexpr auto dwClientState = 0x589FE4;
    constexpr auto dwClientState_ViewAngles = 0x4D90;
    
    // Entity offsets
    constexpr auto m_iHealth = 0x100;
    constexpr auto m_vecOrigin = 0x138;
    constexpr auto m_vecViewOffset = 0x108;
    constexpr auto m_dwBoneMatrix = 0x26A8;
    constexpr auto m_iTeamNum = 0xF4;
    constexpr auto m_bDormant = 0xED;
    constexpr auto m_bSpottedByMask = 0x980;
}

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

    // Game-specific functions
    uintptr_t GetClientModule() const { return clientModule; }
    uintptr_t GetEngineModule() const { return engineModule; }
    HANDLE GetProcessHandle() const { return handle; }

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

// Game structures
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

struct Player {
    bool isEnemy;
    bool isAlive;
    bool isDormant;
    int health;
    Vector3 position;
    Vector3 headPosition;
    int teamNum;
};
