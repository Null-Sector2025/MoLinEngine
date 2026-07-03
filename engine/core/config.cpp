#include "config.h"
#include <fstream>
#include <iostream>

namespace MoLin {

bool Config::LoadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "[Config] Cannot open " << path << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;
        auto eq = line.find('=');
        if (eq != std::string::npos) {
            std::string key = line.substr(0, eq);
            std::string value = line.substr(eq + 1);
            // 去除首尾空格（简化）
            m_Data[key] = value;
        }
    }
    return true;
}

std::string Config::GetString(const std::string& key, const std::string& defaultValue) const {
    auto it = m_Data.find(key);
    return (it != m_Data.end()) ? it->second : defaultValue;
}

int Config::GetInt(const std::string& key, int defaultValue) const {
    std::string val = GetString(key, "");
    if (val.empty()) return defaultValue;
    try { return std::stoi(val); } catch (...) { return defaultValue; }
}

} // namespace MoLin
