#pragma once
#include <Windows.h>

class InputSys {
public:
    static InputSys& Get() {
        static InputSys instance;
        return instance;
    }

    void Initialize();
    bool IsKeyDown(int vk);
    bool WasKeyPressed(int vk);

private:
    InputSys() {}
    BYTE keyState[256];
};
