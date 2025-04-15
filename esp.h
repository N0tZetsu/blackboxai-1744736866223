#pragma once
#include <Windows.h>
#include <d3d9.h>

extern "C" {
    void InitializeESP();
    void RunESP();
    void SetESPEnabled(bool enabled);
    void SetESPFOV(float fov);
    bool IsESPEnabled();
    void SetDevice(LPDIRECT3DDEVICE9 device);
}
