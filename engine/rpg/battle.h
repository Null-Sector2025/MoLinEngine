#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <string>
#include <unordered_map>

namespace MoLin::RPG {

struct BattleStats {
    int hp = 100, maxHp = 100;
    int mp = 50, maxMp = 50;
    int attack = 10, defense = 5;
    int speed = 10;
    int level = 1;
    int expReward = 10;
};

class BattleEntity {
public:
    std::string name;
    BattleStats stats;
    bool isDead() const { return stats.hp <= 0; }
    void takeDamage(int dmg) {
        stats.hp -= dmg;
        if (stats.hp < 0) stats.hp = 0;
    }
    virtual ~BattleEntity() = default;
};

class BattleSkill {
public:
    std::string name;
    std::string description;
    int mpCost = 0;
    int power = 10;
    enum TargetType { SingleEnemy, SingleAlly, AllEnemies, AllAllies };
    TargetType targetType = SingleEnemy;
    std::function<void(BattleEntity& user, BattleEntity& target)> effect;
    std::function<void(BattleEntity& user, std::vector<std::shared_ptr<BattleEntity>>& allies,
                       std::vector<std::shared_ptr<BattleEntity>>& enemies)> areaEffect;
};

struct BattleAction {
    std::shared_ptr<BattleEntity> actor;
    std::shared_ptr<BattleEntity> target;
    std::shared_ptr<BattleSkill> skill;
    bool isDefend = false;
};

enum class BattleState { Idle, PlayerTurn, EnemyTurn, Victory, Defeat };

class BattleSystem {
public:
    virtual ~BattleSystem() = default;
    virtual void StartBattle(const std::vector<std::shared_ptr<BattleEntity>>& playerTeam,
                             const std::vector<std::shared_ptr<BattleEntity>>& enemyTeam) = 0;
    virtual BattleState Update(float delta) = 0;
    virtual void SubmitPlayerAction(int actorIndex, std::shared_ptr<BattleSkill> skill, int targetIndex) = 0;
    virtual bool IsBattleOver() const = 0;
    virtual BattleState GetState() const = 0;
    virtual std::vector<std::shared_ptr<BattleEntity>>& GetAllies() = 0;
    virtual std::vector<std::shared_ptr<BattleEntity>>& GetEnemies() = 0;
    virtual std::vector<BattleAction> GetTurnLog() const = 0;

    static std::shared_ptr<BattleSystem> Create();
};

} // namespace MoLin::RPG
