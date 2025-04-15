#include "aimbot.h"
#include "memory.h"
#include <cmath>
#include <vector>

namespace {
    bool aimbotEnabled = false;
    float aimbotFOV = 5.0f;
    float smoothFactor = 5.0f;
    CRITICAL_SECTION cs;
}

void GetLocalPlayerPos(float& x, float& y, float& z) {
    auto& memory = Memory::Get();
    
    // Read local player
    uintptr_t localPlayer = memory.Read<uintptr_t>(memory.GetClientModule() + Offsets::dwLocalPlayer);
    if (!localPlayer) return;

    // Read position
    Vector3 origin = memory.Read<Vector3>(localPlayer + Offsets::m_vecOrigin);
    Vector3 viewOffset = memory.Read<Vector3>(localPlayer + Offsets::m_vecViewOffset);
    
    // Add view offset to get eye position
    x = origin.x + viewOffset.x;
    y = origin.y + viewOffset.y;
    z = origin.z + viewOffset.z;
}

std::vector<Player> GetEnemyPlayers() {
    std::vector<Player> enemies;
    auto& memory = Memory::Get();
    
    // Read local player first
    uintptr_t localPlayer = memory.Read<uintptr_t>(memory.GetClientModule() + Offsets::dwLocalPlayer);
    if (!localPlayer) return enemies;
    
    int localTeam = memory.Read<int>(localPlayer + Offsets::m_iTeamNum);

    // Loop through entity list
    for (int i = 1; i <= 64; i++) {
        uintptr_t entity = memory.Read<uintptr_t>(memory.GetClientModule() + Offsets::dwEntityList + (i - 1) * 0x10);
        if (!entity) continue;

        // Check if entity is valid
        bool dormant = memory.Read<bool>(entity + Offsets::m_bDormant);
        int health = memory.Read<int>(entity + Offsets::m_iHealth);
        int teamNum = memory.Read<int>(entity + Offsets::m_iTeamNum);
        
        if (dormant || health <= 0 || teamNum == localTeam)
            continue;

        // Get positions
        Vector3 origin = memory.Read<Vector3>(entity + Offsets::m_vecOrigin);
        
        // Get head position from bone matrix
        uintptr_t boneMatrix = memory.Read<uintptr_t>(entity + Offsets::m_dwBoneMatrix);
        Vector3 headPos;
        if (boneMatrix) {
            // Head is usually bone 8
            headPos.x = memory.Read<float>(boneMatrix + 0x30 * 8 + 0x0C);
            headPos.y = memory.Read<float>(boneMatrix + 0x30 * 8 + 0x1C);
            headPos.z = memory.Read<float>(boneMatrix + 0x30 * 8 + 0x2C);
        } else {
            headPos = origin;
            headPos.z += 64.0f; // Approximate head position
        }

        Player player;
        player.isEnemy = true;
        player.isAlive = true;
        player.isDormant = false;
        player.health = health;
        player.position = origin;
        player.headPosition = headPos;
        player.teamNum = teamNum;
        
        enemies.push_back(player);
    }

    return enemies;
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
        // Get current view angles
        uintptr_t clientState = Memory::Get().Read<uintptr_t>(Memory::Get().GetEngineModule() + Offsets::dwClientState);
        if (!clientState) continue;

        Vector3 currentAngles = Memory::Get().Read<Vector3>(clientState + Offsets::dwClientState_ViewAngles);
        
        // Calculate target angles
        float targetPitch, targetYaw;
        CalculateAngle(localX, localY, localZ, target->headPosition.x, target->headPosition.y, target->headPosition.z, targetPitch, targetYaw);
        
        // Smooth the angles
        float newPitch = SmoothAngle(currentAngles.x, targetPitch, smoothFactor);
        float newYaw = SmoothAngle(currentAngles.y, targetYaw, smoothFactor);
        
        // Clamp angles
        if (newPitch > 89.0f) newPitch = 89.0f;
        if (newPitch < -89.0f) newPitch = -89.0f;
        while (newYaw > 180.0f) newYaw -= 360.0f;
        while (newYaw < -180.0f) newYaw += 360.0f;

        // Write new angles
        Vector3 newAngles = {newPitch, newYaw, 0.0f};
        Memory::Get().Write<Vector3>(clientState + Offsets::dwClientState_ViewAngles, newAngles);
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
