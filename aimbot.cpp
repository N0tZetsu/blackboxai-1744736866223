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
    
    // Get local player pawn in CS2
    uintptr_t localPawn = memory.GetLocalPlayerPawn();
    if (!localPawn) return;

    // Read position
    Vector3 origin = memory.Read<Vector3>(localPawn + Offsets::m_vecOrigin);
    Vector3 viewOffset = memory.Read<Vector3>(localPawn + Offsets::m_vecViewOffset);
    
    // Add view offset to get eye position
    x = origin.x + viewOffset.x;
    y = origin.y + viewOffset.y;
    z = origin.z + viewOffset.z;
}

std::vector<Player> GetEnemyPlayers() {
    std::vector<Player> enemies;
    auto& memory = Memory::Get();
    
    // Get local player info
    uintptr_t localController = memory.Read<uintptr_t>(memory.GetClientModule() + Offsets::dwLocalPlayerController);
    if (!localController) return enemies;
    
    uintptr_t localPawn = memory.GetLocalPlayerPawn();
    if (!localPawn) return enemies;
    
    int localTeam = memory.Read<int>(localPawn + Offsets::m_iTeamNum);

    // Loop through entity list (CS2 uses a different structure)
    for (int i = 1; i <= 64; i++) {
        uintptr_t controller = memory.Read<uintptr_t>(memory.GetClientModule() + Offsets::dwEntityList + (i - 1) * 0x8);
        if (!controller) continue;

        // Get pawn handle and resolve to actual pawn
        uintptr_t pawnHandle = memory.Read<uintptr_t>(controller + Offsets::m_hPlayerPawn);
        if (!pawnHandle) continue;

        uintptr_t pawn = memory.GetEntityPawnByIndex(pawnHandle & 0x7FFF);
        if (!pawn) continue;

        // Check if entity is valid
        int health = memory.Read<int>(pawn + Offsets::m_iHealth);
        int teamNum = memory.Read<int>(pawn + Offsets::m_iTeamNum);
        
        if (health <= 0 || teamNum == localTeam)
            continue;

        // Get positions
        Vector3 origin = memory.Read<Vector3>(pawn + Offsets::m_vecOrigin);
        
        // Get head position from game state
        uintptr_t gameSceneNode = memory.Read<uintptr_t>(pawn + 0x310);  // CS2 specific
        uintptr_t boneMatrix = memory.Read<uintptr_t>(gameSceneNode + Offsets::m_dwBoneMatrix);
        Vector3 headPos;
        if (boneMatrix) {
            // Head bone matrix in CS2
            headPos.x = memory.Read<float>(boneMatrix + 0x30 * 6 + 0x0C);
            headPos.y = memory.Read<float>(boneMatrix + 0x30 * 6 + 0x1C);
            headPos.z = memory.Read<float>(boneMatrix + 0x30 * 6 + 0x2C);
        } else {
            headPos = origin;
            headPos.z += 64.0f; // Approximate head position
        }

        Player player;
        player.isEnemy = true;
        player.isAlive = true;
        player.health = health;
        player.position = origin;
        player.headPosition = headPos;
        player.teamNum = teamNum;
        player.controller = controller;
        player.pawn = pawn;
        
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
        // Calculate angle to enemy head
        float pitch, yaw;
        CalculateAngle(localX, localY, localZ, 
                      enemy.headPosition.x, enemy.headPosition.y, enemy.headPosition.z,
                      pitch, yaw);

        // Calculate FOV difference
        float fovDiff = fabsf(yaw); // Simplified for example

        if (fovDiff < bestFOV) {
            bestFOV = fovDiff;
            target = &enemy;
        }
    }

    if (target) {
        // Get current view angles from CS2
        auto& memory = Memory::Get();
        uintptr_t clientState = memory.Read<uintptr_t>(memory.GetEngineModule() + 0x16C8F8); // CS2 offset
        if (!clientState) {
            LeaveCriticalSection(&cs);
            return;
        }

        Vector3 currentAngles = memory.Read<Vector3>(clientState + 0x4D90); // CS2 view angles offset
        
        // Calculate target angles
        float targetPitch, targetYaw;
        CalculateAngle(localX, localY, localZ,
                      target->headPosition.x, target->headPosition.y, target->headPosition.z,
                      targetPitch, targetYaw);
        
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
        memory.Write<Vector3>(clientState + 0x4D90, newAngles);
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
