#include "character.h"
#include "skill.h"
#include <iostream>

namespace MoLin::RPG {

void Character::AddAttribute(const std::string& key, int baseValue, int max) {
    m_Attributes[key] = {baseValue, 0, baseValue, max};
}

Attribute* Character::GetAttribute(const std::string& key) {
    auto it = m_Attributes.find(key);
    return (it != m_Attributes.end()) ? &it->second : nullptr;
}

void Character::ModifyAttribute(const std::string& key, int delta) {
    auto* attr = GetAttribute(key);
    if (attr) {
        attr->current += delta;
        if (attr->current > attr->GetTotal()) attr->current = attr->GetTotal();
        if (attr->current < 0) attr->current = 0;
    }
}

void Character::LearnSkill(std::shared_ptr<Skill> skill) {
    if (!HasSkill(skill->name)) {
        m_Skills.push_back(skill);
    }
}

bool Character::HasSkill(const std::string& name) const {
    for (auto& s : m_Skills) if (s->name == name) return true;
    return false;
}

void Character::UseSkill(const std::string& name, Character& target) {
    for (auto& s : m_Skills) {
        if (s->name == name && s->onUse) {
            // 简单消耗MP检查
            auto* mp = GetAttribute("mp");
            if (mp && mp->current < s->mpCost) {
                std::cout << "[Skill] Not enough MP" << std::endl;
                return;
            }
            s->onUse(*this, target);
            if (mp) mp->current -= s->mpCost;
            return;
        }
    }
    std::cout << "[Skill] Skill not found: " << name << std::endl;
}

} // namespace MoLin::RPG
