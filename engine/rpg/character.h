#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace MoLin::RPG {

struct Attribute {
    int base = 0;
    int bonus = 0;
    int current = 0;
    int maxBase = 9999;
    int GetTotal() const { return base + bonus; }
};

class Skill;
class Character {
public:
    Character(const std::string& name) : m_Name(name) {}

    void AddAttribute(const std::string& key, int baseValue, int max = 9999);
    Attribute* GetAttribute(const std::string& key);
    void ModifyAttribute(const std::string& key, int delta);

    void LearnSkill(std::shared_ptr<Skill> skill);
    bool HasSkill(const std::string& name) const;
    void UseSkill(const std::string& name, Character& target);

    std::string m_Name;
    int m_Level = 1;
    int m_Exp = 0;
    std::unordered_map<std::string, Attribute> m_Attributes;
    std::vector<std::shared_ptr<Skill>> m_Skills;
};

} // namespace MoLin::RPG
