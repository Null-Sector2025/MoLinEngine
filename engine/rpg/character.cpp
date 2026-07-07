#include "rpg/character.h"
#include "rpg/inventory.h"
#include <cmath>
#include <algorithm>

namespace MoLin {
namespace RPG {

Character::Character() 
    : m_level(1), m_hp(100), m_maxHp(100), m_mp(50), m_maxMp(50),
      m_atk(10), m_def(5), m_exp(0), m_nextLevelExp(100) {
    m_inventory = new Inventory();
}

Character::~Character() {
    delete m_inventory;
}

void Character::SetLevel(int level) {
    m_level = level;
    m_maxHp = 100 + level * 20;
    m_maxMp = 50 + level * 10;
    m_atk = 10 + level * 3;
    m_def = 5 + level * 2;
}

int Character::GetLevel() const {
    return m_level;
}

void Character::AddExp(int exp) {
    m_exp += exp;
    while (m_exp >= m_nextLevelExp) {
        m_exp -= m_nextLevelExp;
        SetLevel(m_level + 1);
        m_nextLevelExp = static_cast<int>(m_nextLevelExp * 1.5f);
    }
}

void Character::TakeDamage(int damage) {
    int actualDamage = std::max(0, damage - m_def);
    m_hp = std::max(0, m_hp - actualDamage);
}

void Character::Heal(int amount) {
    m_hp = std::min(m_maxHp, m_hp + amount);
}

void Character::UseMp(int amount) {
    m_mp = std::max(0, m_mp - amount);
}

void Character::RestoreMp(int amount) {
    m_mp = std::min(m_maxMp, m_mp + amount);
}

void Character::SetPosition(float x, float y) {
    m_x = x;
    m_y = y;
}

void Character::GetPosition(float& x, float& y) const {
    x = m_x;
    y = m_y;
}

Inventory* Character::GetInventory() {
    return m_inventory;
}

const Inventory* Character::GetInventory() const {
    return m_inventory;
}

} // namespace RPG
} // namespace MoLin
