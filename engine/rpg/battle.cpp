#include "battle.h"
#include <iostream>
#include <algorithm>

namespace MoLin::RPG {

class TurnBasedBattle : public BattleSystem {
public:
    void StartBattle(const std::vector<std::shared_ptr<BattleEntity>>& allies,
                     const std::vector<std::shared_ptr<BattleEntity>>& enemies) override {
        m_Allies = allies; m_Enemies = enemies; m_BattleOver = false;
    }
    void ExecuteCommand(int actorIndex, const BattleCommand& cmd) override {
        if (m_BattleOver) return;
        if (!m_Enemies.empty()) cmd.execute(m_Allies, m_Enemies);
        auto allDead = [](auto& list) {
            return std::all_of(list.begin(), list.end(), [](auto& e){ return e->stats.hp <= 0; });
        };
        if (allDead(m_Enemies)) m_BattleOver = true;
        else if (allDead(m_Allies)) m_BattleOver = true;
    }
    bool IsBattleOver() override { return m_BattleOver; }
private:
    std::vector<std::shared_ptr<BattleEntity>> m_Allies, m_Enemies;
    bool m_BattleOver = false;
};

std::shared_ptr<BattleSystem> CreateBattleSystem() {
    return std::make_shared<TurnBasedBattle>();
}

} // namespace MoLin::RPG
