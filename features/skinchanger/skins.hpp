#pragma once

class CSkinChanger {
public:
    static CSkinChanger& Get() {
        static CSkinChanger instance;
        return instance;
    }

    void Initialize();
    void Run();

private:
    CSkinChanger() {}
};
