#pragma once
#include <string>
#include <unordered_map>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

namespace MoLin {

class SaveSystem {
public:
    SaveSystem() = default;

    void SetString(const std::string& key, const std::string& value);
    void SetInt(const std::string& key, int value);
    void SetFloat(const std::string& key, float value);

    std::string GetString(const std::string& key, const std::string& def = "") const;
    int GetInt(const std::string& key, int def = 0) const;
    float GetFloat(const std::string& key, float def = 0.0f) const;

    bool SaveToFile(const std::string& filename);
    bool LoadFromFile(const std::string& filename);

private:
    std::unordered_map<std::string, std::string> m_Data;
};

} // namespace MoLin
