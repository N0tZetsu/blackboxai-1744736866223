#include "esp.h"
#include <mutex>

static bool espEnabled = false;
static float espFOV = 90.0f;
static std::mutex mtx;

void InitializeESP() {
    // Initialization code if needed
}

void RunESP() {
    if (!espEnabled) return;

    // TODO: Implement ESP rendering here
    // This will be implemented when we have the DirectX device context
    // Will draw boxes, health bars, names, etc. around players
}

void SetESPEnabled(bool enabled) {
    std::lock_guard<std::mutex> lock(mtx);
    espEnabled = enabled;
}

void SetESPFOV(float fov) {
    std::lock_guard<std::mutex> lock(mtx);
    espFOV = fov;
}

bool IsESPEnabled() {
    std::lock_guard<std::mutex> lock(mtx);
    return espEnabled;
}
