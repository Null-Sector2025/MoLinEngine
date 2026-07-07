#pragma once
#include <string>
#include <vector>

namespace MoLin {
namespace RPG {

class Inventory;

class Character {
public:
    Character();
    ~Character();

    // 等级
    void SetLevel(int level);
    int GetLevel() const;
    void AddExp(int exp);

    // HP/MP
    int GetHp() const { return m_hp; }
    int GetMaxHp() const { return m_maxHp; }
    void TakeDamage(int damage);
    void Heal(int amount);
    
    int GetMp() const { return m_mp; }
    int GetMaxMp() const { return m_maxMp; }
    void UseMp(int amount);
    void RestoreMp(int amount);

    // 属性
    int GetAttack() const { return m_atk; }
    int GetDefense() const { return m_def; }
    int GetExp() const { return m_exp; }
    int GetNextLevelExp() const { return m_nextLevelExp; }

    // 位置
    void SetPosition(float x, float y);
    void GetPosition(float& x, float& y) const;

    // 背包
    Inventory* GetInventory();
    const Inventory* GetInventory() const;

private:
    int m_level;
    int m_hp, m_maxHp;
    int m_mp, m_maxMp;
    int m_atk, m_def;
    int m_exp, m_nextLevelExp;
    float m_x = 0, m_y = 0;
    Inventory* m_inventory = nullptr;
};

} // namespace RPG
} // namespace MoLin
