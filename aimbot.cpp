#include "aimbot.h"
#include <cmath>
#include <vector>

namespace {
    bool aimbotEnabled = false;
    float aimbotFOV = 5.0f;
    float smoothFactor = 5.0f;
    CRITICAL_SECTION cs;

    // Dummy player structure for demonstration
    struct Player {
        bool isEnemy;
        bool isAlive;
        float x, y, z; // Position
        float headX, headY, headZ; // Head position for aimbot
    };
}

// Dummy function to get local player position
void GetLocalPlayerPos(float& x, float& y, float& z) {
    // TODO: Implement actual memory reading
    x = 0.0f; y = 0.0f; z = 0.0f;
}

// Dummy function to get enemy players
std::vector<Player> GetEnemyPlayers() {
    // TODO: Implement actual memory reading
    return std::vector<Player>{};
}

// Calculate angle to target for aimbot
void CalculateAngle(float srcX, float srcY, float srcZ, float dstX, float dstY, float dstZ, float& pitch, float& yaw) {
    float deltaX = dstX - srcX;
    float deltaY = dstY - srcY;
    float deltaZ = dstZ - srcZ;
    float hyp = sqrtf(deltaX * deltaX + deltaY * deltaY);
    pitch = -atan2f(deltaZ, hyp) * (180.0f / 3.14159265358979323846f);
    yaw = atan2f(deltaY, deltaX) * (180.0f / 3.14159265358979323846f);
}

// Smoothly move current angle towards target angle
float SmoothAngle(float current, float target, float smooth) {
    float delta = target - current;
    if (delta > 180.0f) delta -= 360.0f;
    if (delta < -180.0f) delta += 360.0f;
    return current + delta / smooth;
}

void InitializeAimbot() {
    InitializeCriticalSection(&cs);
}

void RunAimbot() {
    EnterCriticalSection(&cs);
    if (!aimbotEnabled) {
        LeaveCriticalSection(&cs);
        return;
    }

    float localX, localY, localZ;
    GetLocalPlayerPos(localX, localY, localZ);

    auto enemies = GetEnemyPlayers();

    float bestFOV = aimbotFOV;
    Player* target = nullptr;

    // Find closest enemy within FOV
    for (auto& enemy : enemies) {
        if (!enemy.isAlive || !enemy.isEnemy) continue;

        // Calculate angle to enemy head
        float pitch, yaw;
        CalculateAngle(localX, localY, localZ, enemy.headX, enemy.headY, enemy.headZ, pitch, yaw);

        // Calculate FOV difference (simplified)
        float fovDiff = fabsf(yaw); // Simplified for example

        if (fovDiff < bestFOV) {
            bestFOV = fovDiff;
            target = &enemy;
        }
    }

    if (target) {
        // Smooth aim towards target
        // TODO: Implement actual view angle writing
    }
    
    LeaveCriticalSection(&cs);
}

void SetAimbotEnabled(bool enabled) {
    EnterCriticalSection(&cs);
    aimbotEnabled = enabled;
    LeaveCriticalSection(&cs);
}

void SetAimbotFOV(float fov) {
    EnterCriticalSection(&cs);
    aimbotFOV = fov;
    LeaveCriticalSection(&cs);
}

void SetSmoothFactor(float smooth) {
    EnterCriticalSection(&cs);
    smoothFactor = smooth;
    LeaveCriticalSection(&cs);
}

bool IsAimbotEnabled() {
    EnterCriticalSection(&cs);
    bool enabled = aimbotEnabled;
    LeaveCriticalSection(&cs);
    return enabled;
}
