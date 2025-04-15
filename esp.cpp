#include "esp.h"
#include <Windows.h>

namespace {
    bool espEnabled = false;
    float espFOV = 90.0f;
    CRITICAL_SECTION cs;
}

void InitializeESP() {
    InitializeCriticalSection(&cs);
}

void RunESP() {
    EnterCriticalSection(&cs);
    if (!espEnabled) {
        LeaveCriticalSection(&cs);
        return;
    }

    // TODO: Implement ESP rendering here
    // This will be implemented when we have the DirectX device context
    // Will draw boxes, health bars, names, etc. around players

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
