#include "save_system.h"
#include <fstream>
#include <iostream>

namespace MoLin {

void SaveSystem::SetString(const std::string& key, const std::string& value) { m_Data[key] = value; }
void SaveSystem::SetInt(const std::string& key, int value) { m_Data[key] = std::to_string(value); }
void SaveSystem::SetFloat(const std::string& key, float value) { m_Data[key] = std::to_string(value); }

std::string SaveSystem::GetString(const std::string& key, const std::string& def) const {
    auto it = m_Data.find(key);
    return (it != m_Data.end()) ? it->second : def;
}

int SaveSystem::GetInt(const std::string& key, int def) const {
    auto it = m_Data.find(key);
    if (it != m_Data.end()) return std::stoi(it->second);
    return def;
}

float SaveSystem::GetFloat(const std::string& key, float def) const {
    auto it = m_Data.find(key);
    if (it != m_Data.end()) return std::stof(it->second);
    return def;
}

bool SaveSystem::SaveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) return false;
    file << "return {\n";
    for (const auto& pair : m_Data) {
        file << "  [\"" << pair.first << "\"] = \"" << pair.second << "\",\n";
    }
    file << "}\n";
    return true;
}

bool SaveSystem::LoadFromFile(const std::string& filename) {
    lua_State* L = luaL_newstate();
    if (luaL_dofile(L, filename.c_str()) != LUA_OK) {
        lua_close(L);
        return false;
    }
    if (!lua_istable(L, -1)) {
        lua_close(L);
        return false;
    }
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        std::string key = lua_tostring(L, -2);
        std::string value = lua_tostring(L, -1);
        m_Data[key] = value;
        lua_pop(L, 1);
    }
    lua_close(L);
    return true;
}

} // namespace MoLin
