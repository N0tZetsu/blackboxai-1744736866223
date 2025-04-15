#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d9.h>
#include <d3dx9.h>

// CS2 specific offsets and structures
namespace SDK {
    // Client offsets
    constexpr auto dwLocalPlayerController = 0x1810F48;
    constexpr auto dwEntityList = 0x17C1950;
    constexpr auto dwViewMatrix = 0x1820150;
    constexpr auto dwGlobalVars = 0x169C0D0;
    
    // Entity offsets
    constexpr auto m_hPlayerPawn = 0x7EC;
    constexpr auto m_iHealth = 0x32C;
    constexpr auto m_iTeamNum = 0x3BF;
    constexpr auto m_vecOrigin = 0x1224;
    constexpr auto m_vecViewOffset = 0x108;
    constexpr auto m_dwBoneMatrix = 0x1124;
    constexpr auto m_bSpottedByMask = 0x1698;

    // Basic structures
    struct Vector3 {
        float x, y, z;
    };

    struct Matrix4x4 {
        float m[4][4];
    };

    // Interface classes (simplified for example)
    class IBaseClientDLL {
    public:
        virtual ~IBaseClientDLL() {}
    };

    class IClientEntityList {
    public:
        virtual ~IClientEntityList() {}
    };

    class IVEngineClient {
    public:
        virtual ~IVEngineClient() {}
    };
}

// Global interfaces
namespace Interfaces {
    void Initialize();
    void Dump();
}

// Global variables
namespace Globals {
    extern bool Unload;
}
