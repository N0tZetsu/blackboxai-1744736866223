#pragma once
#include <Windows.h>

extern "C" {
    void InitializeESP();
    void RunESP();
    void SetESPEnabled(bool enabled);
    void SetESPFOV(float fov);
    bool IsESPEnabled();
}
