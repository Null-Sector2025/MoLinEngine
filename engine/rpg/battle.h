#pragma once
#include <vector>
#include <memory>

namespace MoLin::RPG {

struct BattleStats {
    int hp = 100;
    int maxHp = 100;
    int attack = 10;
    int defense = 5;
    int speed = 10;
};

class BattleEntity {
public:
    std::string name;
    BattleStats stats;
    virtual ~BattleEntity() = default;
    // 技能等由子类实现
};

class BattleCommand {
public:
    std::string name;
    std::function<void(std::vector<std::shared_ptr<BattleEntity>>& allies,
                       std::vector<std::shared_ptr<BattleEntity>>& enemies)> execute;
};

class BattleSystem {
public:
    virtual ~BattleSystem() = default;
    virtual void StartBattle(const std::vector<std::shared_ptr<BattleEntity>>& allies,
                             const std::vector<std::shared_ptr<BattleEntity>>& enemies) = 0;
    virtual void ExecuteCommand(int actorIndex, const BattleCommand& cmd) = 0;
    virtual bool IsBattleOver() = 0;
};

} // namespace MoLin::RPG
