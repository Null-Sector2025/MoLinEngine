#pragma once
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>

namespace MoLin {

class SimpleJson {
public:
    bool Parse(const std::string& json);
    std::string GetString(const std::string& key, const std::string& def = "") const;
    int GetInt(const std::string& key, int def = 0) const;
    float GetFloat(const std::string& key, float def = 0.0f) const;
    bool HasKey(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> m_Values;
};

} // namespace MoLin
