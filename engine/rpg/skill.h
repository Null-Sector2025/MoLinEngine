#pragma once
#include <string>
#include <functional>

namespace MoLin::RPG {

class Skill {
public:
    std::string name;
    std::string description;
    int mpCost = 0;
    int power = 0;
    int level = 1;
    int maxLevel = 10;

    using SkillEffect = std::function<void(class Character& user, class Character& target)>;
    SkillEffect onUse;
};

} // namespace MoLin::RPG
