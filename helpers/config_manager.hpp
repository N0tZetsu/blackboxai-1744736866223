#pragma once
#include <string>
#include <map>

class CConfig {
public:
    static CConfig& Get() {
        static CConfig instance;
        return instance;
    }

    void Initialize();
    void Save();
    void Load();

private:
    CConfig() {}
    std::map<std::string, std::string> settings;
};
