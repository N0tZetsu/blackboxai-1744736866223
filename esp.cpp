#include "esp.h"
#include <mutex>
#include <thread>

namespace {
    bool espEnabled = false;
    float espFOV = 90.0f;
    std::mutex mtx;
}

void InitializeESP() {
    // Initialization code if needed
}

void RunESP() {
    std::lock_guard<std::mutex> lock(mtx);
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
