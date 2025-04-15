#include "esp.h"
#include "memory.h"
#include <vector>

namespace {
    bool espEnabled = false;
    float espFOV = 90.0f;
    CRITICAL_SECTION cs;
    
    // Vulkan rendering context
    VkDevice device = VK_NULL_HANDLE;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    float viewportWidth = 0.0f;
    float viewportHeight = 0.0f;

    // ESP colors (RGBA format for Vulkan)
    const uint32_t COLOR_T = 0x00FF00FF;    // Green
    const uint32_t COLOR_CT = 0x0000FFFF;   // Blue
    const uint32_t COLOR_HP = 0xFF0000FF;   // Red

    // Helper function to draw line (simplified)
    void DrawLine(float x1, float y1, float x2, float y2, uint32_t color) {
        if (!cmdBuffer) return;
        
        // In real implementation, this would use vkCmdDraw to draw the line
        // This is a placeholder for demonstration
        VkViewport viewport{};
        viewport.width = viewportWidth;
        viewport.height = viewportHeight;
        vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
    }

    // Helper function to draw rectangle
    void DrawRect(float x, float y, float w, float h, uint32_t color) {
        // Draw four lines to form a rectangle
        DrawLine(x, y, x + w, y, color);         // Top
        DrawLine(x, y + h, x + w, y + h, color); // Bottom
        DrawLine(x, y, x, y + h, color);         // Left
        DrawLine(x + w, y, x + w, y + h, color); // Right
    }

    // Helper function to draw filled rectangle
    void DrawFilledRect(float x, float y, float w, float h, uint32_t color) {
        if (!cmdBuffer) return;
        
        // In real implementation, this would use vkCmdDraw to draw a filled rectangle
        VkViewport viewport{};
        viewport.width = viewportWidth;
        viewport.height = viewportHeight;
        vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
    }

    // Helper function to world-to-screen coordinate conversion for CS2
    bool WorldToScreen(const Vector3& pos, float* screen) {
        auto& memory = Memory::Get();
        Matrix4x4 viewMatrix = memory.GetViewMatrix();

        // CS2 uses a different projection matrix setup
        float w = viewMatrix.m[3][0] * pos.x + viewMatrix.m[3][1] * pos.y + 
                 viewMatrix.m[3][2] * pos.z + viewMatrix.m[3][3];

        if (w < 0.01f)
            return false;

        float x = viewMatrix.m[0][0] * pos.x + viewMatrix.m[0][1] * pos.y + 
                 viewMatrix.m[0][2] * pos.z + viewMatrix.m[0][3];
        float y = viewMatrix.m[1][0] * pos.x + viewMatrix.m[1][1] * pos.y + 
                 viewMatrix.m[1][2] * pos.z + viewMatrix.m[1][3];

        // Convert to screen coordinates
        screen[0] = (viewportWidth / 2.0f) * (1.0f + x / w);
        screen[1] = (viewportHeight / 2.0f) * (1.0f - y / w);

        return true;
    }
}

void InitializeESP() {
    InitializeCriticalSection(&cs);
}

void SetVulkanDevice(VkDevice dev, VkCommandBuffer cmd) {
    device = dev;
    cmdBuffer = cmd;
}

void SetViewport(float width, float height) {
    viewportWidth = width;
    viewportHeight = height;
}

void RunESP() {
    EnterCriticalSection(&cs);
    if (!espEnabled || !cmdBuffer) {
        LeaveCriticalSection(&cs);
        return;
    }

    auto& memory = Memory::Get();
    auto enemies = GetEnemyPlayers();

    for (const auto& player : enemies) {
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
        DrawRect(x, y, width, height, 
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
