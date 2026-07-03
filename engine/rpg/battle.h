#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <string>

namespace MoLin::RPG {

struct BattleStats {
    int hp = 100, maxHp = 100;
    int attack = 10, defense = 5, speed = 10;
};

class BattleEntity {
public:
    std::string name;
    BattleStats stats;
    virtual ~BattleEntity() = default;
};

class BattleCommand {
public:
    std::string name;
    std::function<void(std::vector<std::shared_ptr<BattleEntity>>&,
                       std::vector<std::shared_ptr<BattleEntity>>&)> execute;
};

class BattleSystem {
public:
    virtual ~BattleSystem() = default;
    virtual void StartBattle(const std::vector<std::shared_ptr<BattleEntity>>&,
                             const std::vector<std::shared_ptr<BattleEntity>>&) = 0;
    virtual void ExecuteCommand(int actorIndex, const BattleCommand& cmd) = 0;
    virtual bool IsBattleOver() = 0;
};

std::shared_ptr<BattleSystem> CreateBattleSystem();

} // namespace MoLin::RPG
