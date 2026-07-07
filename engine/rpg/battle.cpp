#include "battle.h"
#include <algorithm>
#include <iostream>

namespace MoLin::RPG {

class TurnBasedBattle : public BattleSystem {
public:
    void StartBattle(const std::vector<std::shared_ptr<BattleEntity>>& playerTeam,
                     const std::vector<std::shared_ptr<BattleEntity>>& enemyTeam) override {
        m_Allies = playerTeam;
        m_Enemies = enemyTeam;
        m_State = BattleState::PlayerTurn;
        m_TurnLog.clear();
        std::cout << "[Battle] 战斗开始！敌人队伍：" << m_Enemies.size() << " 个目标\n";
    }

    BattleState Update(float delta) override {
        if (m_State == BattleState::PlayerTurn) {
            // 等待玩家输入
            return m_State;
        }
        if (m_State == BattleState::EnemyTurn) {
            ExecuteEnemyAction();
            m_State = CheckBattleEnd();
            if (m_State == BattleState::PlayerTurn) {
                // 回合循环
            }
            return m_State;
        }
        return m_State;
    }

    void SubmitPlayerAction(int actorIndex, std::shared_ptr<BattleSkill> skill, int targetIndex) override {
        if (m_State != BattleState::PlayerTurn) return;
        if (actorIndex < 0 || actorIndex >= m_Allies.size()) return;

        auto& actor = m_Allies[actorIndex];
        if (actor->isDead()) return;

        BattleAction action;
        action.actor = actor;
        action.skill = skill;

        if (skill) {
            if (actor->stats.mp < skill->mpCost) return;
            actor->stats.mp -= skill->mpCost;

            if (skill->targetType == BattleSkill::SingleEnemy || skill->targetType == BattleSkill::AllEnemies) {
                if (targetIndex >= 0 && targetIndex < m_Enemies.size()) {
                    action.target = m_Enemies[targetIndex];
                }
            } else {
                if (targetIndex >= 0 && targetIndex < m_Allies.size()) {
                    action.target = m_Allies[targetIndex];
                }
            }
            if (skill->effect && action.target) {
                skill->effect(*actor, *action.target);
            }
            if (skill->areaEffect) {
                skill->areaEffect(*actor, m_Allies, m_Enemies);
            }
        } else {
            // 普通攻击
            if (targetIndex >= 0 && targetIndex < m_Enemies.size()) {
                action.target = m_Enemies[targetIndex];
                int dmg = std::max(1, actor->stats.attack - m_Enemies[targetIndex]->stats.defense);
                m_Enemies[targetIndex]->takeDamage(dmg);
                std::cout << "[Battle] " << actor->name << " 攻击 " << m_Enemies[targetIndex]->name
                          << " 造成 " << dmg << " 点伤害\n";
            }
        }
        m_TurnLog.push_back(action);
        m_State = BattleState::EnemyTurn;
    }

    bool IsBattleOver() const override {
        return m_State == BattleState::Victory || m_State == BattleState::Defeat;
    }

    BattleState GetState() const override { return m_State; }
    std::vector<std::shared_ptr<BattleEntity>>& GetAllies() override { return m_Allies; }
    std::vector<std::shared_ptr<BattleEntity>>& GetEnemies() override { return m_Enemies; }
    std::vector<BattleAction> GetTurnLog() const override { return m_TurnLog; }

private:
    void ExecuteEnemyAction() {
        // 简单 AI：每个敌人攻击随机存活队友
        for (auto& enemy : m_Enemies) {
            if (enemy->isDead()) continue;
            // 找一个活着的队友
            std::vector<std::shared_ptr<BattleEntity>> aliveAllies;
            for (auto& ally : m_Allies) {
                if (!ally->isDead()) aliveAllies.push_back(ally);
            }
            if (aliveAllies.empty()) break;
            int idx = rand() % aliveAllies.size();
            int dmg = std::max(1, enemy->stats.attack - aliveAllies[idx]->stats.defense);
            aliveAllies[idx]->takeDamage(dmg);
            std::cout << "[Battle] " << enemy->name << " 攻击 " << aliveAllies[idx]->name
                      << " 造成 " << dmg << " 点伤害\n";
        }
    }

    BattleState CheckBattleEnd() {
        bool allEnemiesDead = std::all_of(m_Enemies.begin(), m_Enemies.end(),
            [](auto& e) { return e->isDead(); });
        bool allAlliesDead = std::all_of(m_Allies.begin(), m_Allies.end(),
            [](auto& a) { return a->isDead(); });
        if (allEnemiesDead) {
            std::cout << "[Battle] 胜利！\n";
            return BattleState::Victory;
        }
        if (allAlliesDead) {
            std::cout << "[Battle] 败北...\n";
            return BattleState::Defeat;
        }
        return BattleState::PlayerTurn;
    }

    std::vector<std::shared_ptr<BattleEntity>> m_Allies;
    std::vector<std::shared_ptr<BattleEntity>> m_Enemies;
    BattleState m_State = BattleState::Idle;
    std::vector<BattleAction> m_TurnLog;
};

std::shared_ptr<BattleSystem> BattleSystem::Create() {
    return std::make_shared<TurnBasedBattle>();
}

} // namespace MoLin::RPG
