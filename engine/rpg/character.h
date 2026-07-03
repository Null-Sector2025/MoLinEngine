#pragma once
#include <string>
#include <map>
#include <vector>

namespace MoLin::RPG {

struct Attribute {
    std::string name;
    int baseValue = 0;
    int currentValue = 0;
    int maxValue = 9999;
};

class Character {
public:
    Character(const std::string& name) : m_Name(name) {}

    void AddAttribute(const std::string& key, int base, int max = 9999);
    Attribute* GetAttribute(const std::string& key);

    int GetLevel() const { return m_Level; }
    void SetLevel(int level) { m_Level = level; }

    std::string m_Name;
    int m_Level = 1;
    std::map<std::string, Attribute> m_Attributes;
};

} // namespace MoLin::RPG
