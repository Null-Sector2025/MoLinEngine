#pragma once
#include <string>
#include <unordered_map>
#include <sstream>

namespace MoLin {

class Config {
public:
    bool LoadFromFile(const std::string& path);
    std::string GetString(const std::string& key, const std::string& defaultValue = "") const;
    int GetInt(const std::string& key, int defaultValue = 0) const;

private:
    std::unordered_map<std::string, std::string> m_Data;
};

} // namespace MoLin
