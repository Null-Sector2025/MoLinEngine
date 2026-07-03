#pragma once
#include <string>
#include <functional>

namespace MoLin::RPG {

class Skill {
public:
    std::string name;
    int mpCost = 0;
    int power = 0;
    std::string description;

    using SkillEffect = std::function<void(class BattleEntity& user, class BattleEntity& target)>;
    SkillEffect onUse;
};

} // namespace MoLin::RPG
