#include "config.h"
#include <fstream>

namespace MoLin {

bool Config::LoadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) return false;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto eq = line.find('=');
        if (eq != std::string::npos) m_Data[line.substr(0, eq)] = line.substr(eq + 1);
    }
    return true;
}
std::string Config::GetString(const std::string& key, const std::string& def) const {
    auto it = m_Data.find(key); return (it != m_Data.end()) ? it->second : def;
}
int Config::GetInt(const std::string& key, int def) const {
    auto it = m_Data.find(key); if (it == m_Data.end()) return def;
    try { return std::stoi(it->second); } catch (...) { return def; }
}

} // namespace MoLin
