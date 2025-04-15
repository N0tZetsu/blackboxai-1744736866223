#pragma once
#include <Windows.h>

extern "C" {
    void InitializeAimbot();
    void RunAimbot();
    void SetAimbotEnabled(bool enabled);
    void SetAimbotFOV(float fov);
    void SetSmoothFactor(float smooth);
    bool IsAimbotEnabled();
}
