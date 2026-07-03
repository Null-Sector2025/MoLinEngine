#include "battle.h"
#include <iostream>
#include <algorithm>

namespace MoLin::RPG {

class TurnBasedBattle : public BattleSystem {
public:
    void StartBattle(const std::vector<std::shared_ptr<BattleEntity>>& allies,
                     const std::vector<std::shared_ptr<BattleEntity>>& enemies) override {
        m_Allies = allies;
        m_Enemies = enemies;
        m_CurrentTurn = 0;
        m_BattleOver = false;
        std::cout << "[Battle] Battle started!" << std::endl;
    }

    void ExecuteCommand(int actorIndex, const BattleCommand& cmd) override {
        if (m_BattleOver) return;
        // 简单实现：对第一个敌人执行命令
        if (!m_Enemies.empty()) {
            std::cout << "[Battle] " << m_Allies[actorIndex]->name << " uses " << cmd.name << std::endl;
            cmd.execute(m_Allies, m_Enemies);
        }
        // 检查战斗是否结束
        auto allDead = [](const std::vector<std::shared_ptr<BattleEntity>>& list) {
            return std::all_of(list.begin(), list.end(), [](auto& e){ return e->stats.hp <= 0; });
        };
        if (allDead(m_Enemies)) {
            std::cout << "[Battle] Victory!" << std::endl;
            m_BattleOver = true;
        } else if (allDead(m_Allies)) {
            std::cout << "[Battle] Defeat..." << std::endl;
            m_BattleOver = true;
        }
    }

    bool IsBattleOver() override { return m_BattleOver; }

private:
    std::vector<std::shared_ptr<BattleEntity>> m_Allies;
    std::vector<std::shared_ptr<BattleEntity>> m_Enemies;
    int m_CurrentTurn = 0;
    bool m_BattleOver = false;
};

std::shared_ptr<BattleSystem> CreateBattleSystem() {
    return std::make_shared<TurnBasedBattle>();
}

} // namespace MoLin::RPG
