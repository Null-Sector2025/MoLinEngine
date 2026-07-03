#include "json.h"

namespace MoLin {

bool SimpleJson::Parse(const std::string& json) {
    std::string data = json;
    // 去掉首尾空白和花括号
    auto start = data.find('{');
    auto end = data.rfind('}');
    if (start == std::string::npos || end == std::string::npos) return false;
    data = data.substr(start+1, end-start-1);

    std::stringstream ss(data);
    std::string token;
    while (std::getline(ss, token, ',')) {
        // 简单分割 key:value
        auto colon = token.find(':');
        if (colon == std::string::npos) continue;
        std::string key = token.substr(0, colon);
        std::string value = token.substr(colon+1);

        // 去除引号和空格
        auto trim = [](std::string& s) {
            s.erase(0, s.find_first_not_of(" \t\n\r\""));
            s.erase(s.find_last_not_of(" \t\n\r\"")+1);
        };
        trim(key);
        trim(value);

        m_Values[key] = value;
    }
    return true;
}

std::string SimpleJson::GetString(const std::string& key, const std::string& def) const {
    auto it = m_Values.find(key);
    return (it != m_Values.end()) ? it->second : def;
}

int SimpleJson::GetInt(const std::string& key, int def) const {
    auto it = m_Values.find(key);
    if (it != m_Values.end()) {
        try { return std::stoi(it->second); } catch(...) {}
    }
    return def;
}

float SimpleJson::GetFloat(const std::string& key, float def) const {
    auto it = m_Values.find(key);
    if (it != m_Values.end()) {
        try { return std::stof(it->second); } catch(...) {}
    }
    return def;
}

bool SimpleJson::HasKey(const std::string& key) const {
    return m_Values.find(key) != m_Values.end();
}

} // namespace MoLin
