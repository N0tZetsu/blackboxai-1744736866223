#include "esp.h"
#include "memory.h"
#include <d3d9.h>
#include <d3dx9.h>

namespace {
    bool espEnabled = false;
    float espFOV = 90.0f;
    CRITICAL_SECTION cs;
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    D3DVIEWPORT9 viewport;
    float viewMatrix[4][4];

    // ESP colors
    const D3DCOLOR COLOR_T = D3DCOLOR_RGBA(0, 255, 0, 255);    // Terrorist
    const D3DCOLOR COLOR_CT = D3DCOLOR_RGBA(0, 0, 255, 255);   // Counter-Terrorist
    const D3DCOLOR COLOR_HP = D3DCOLOR_RGBA(255, 0, 0, 255);   // Health bar

    // Helper function to draw outlined rectangle
    void DrawOutlinedRect(int x, int y, int w, int h, D3DCOLOR color) {
        D3DRECT rect[4] = {
            {x, y, x + w, y + 1},     // Top
            {x, y + h, x + w, y + h + 1}, // Bottom
            {x, y, x + 1, y + h},     // Left
            {x + w, y, x + w + 1, y + h}  // Right
        };
        pDevice->Clear(4, rect, D3DCLEAR_TARGET, color, 0.0f, 0);
    }

    // Helper function to draw filled rectangle
    void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
        D3DRECT rect = {x, y, x + w, y + h};
        pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0.0f, 0);
    }

    // Helper function to world-to-screen coordinate conversion
    bool WorldToScreen(const Vector3& pos, float* screen) {
        // Get view matrix from game
        auto& memory = Memory::Get();
        memcpy(viewMatrix, 
            (void*)(memory.GetClientModule() + Offsets::dwViewMatrix),
            sizeof(viewMatrix));

        // Matrix-vector multiplication
        float w = viewMatrix[3][0] * pos.x + viewMatrix[3][1] * pos.y + 
                 viewMatrix[3][2] * pos.z + viewMatrix[3][3];

        if (w < 0.01f)
            return false;

        // Normalize coordinates
        float x = viewMatrix[0][0] * pos.x + viewMatrix[0][1] * pos.y + 
                 viewMatrix[0][2] * pos.z + viewMatrix[0][3];
        float y = viewMatrix[1][0] * pos.x + viewMatrix[1][1] * pos.y + 
                 viewMatrix[1][2] * pos.z + viewMatrix[1][3];

        // Convert to screen coordinates
        screen[0] = (viewport.Width / 2.0f) * (1.0f + x / w);
        screen[1] = (viewport.Height / 2.0f) * (1.0f - y / w);

        return true;
    }
}

void InitializeESP() {
    InitializeCriticalSection(&cs);
}

void SetDevice(LPDIRECT3DDEVICE9 device) {
    pDevice = device;
    if (pDevice) {
        pDevice->GetViewport(&viewport);
    }
}

void RunESP() {
    EnterCriticalSection(&cs);
    if (!espEnabled || !pDevice) {
        LeaveCriticalSection(&cs);
        return;
    }

    auto enemies = GetEnemyPlayers();
    for (const auto& player : enemies) {
        if (player.isDormant)
            continue;

        // Convert 3D positions to 2D screen coordinates
        float screenPos[2], headScreenPos[2];
        if (!WorldToScreen(player.position, screenPos) || 
            !WorldToScreen(player.headPosition, headScreenPos))
            continue;

        // Calculate box dimensions
        float height = screenPos[1] - headScreenPos[1];
        float width = height / 2.0f;
        float x = headScreenPos[0] - width / 2.0f;
        float y = headScreenPos[1];

        // Draw box
        DrawOutlinedRect(x, y, width, height, 
            player.teamNum == 2 ? COLOR_T : COLOR_CT);

        // Draw health bar
        float healthHeight = height * (player.health / 100.0f);
        DrawFilledRect(x - 5, y + height - healthHeight, 3, 
            healthHeight, COLOR_HP);
    }

    LeaveCriticalSection(&cs);
}

void SetESPEnabled(bool enabled) {
    EnterCriticalSection(&cs);
    espEnabled = enabled;
    LeaveCriticalSection(&cs);
}

void SetESPFOV(float fov) {
    EnterCriticalSection(&cs);
    espFOV = fov;
    LeaveCriticalSection(&cs);
}

bool IsESPEnabled() {
    EnterCriticalSection(&cs);
    bool enabled = espEnabled;
    LeaveCriticalSection(&cs);
    return enabled;
}
