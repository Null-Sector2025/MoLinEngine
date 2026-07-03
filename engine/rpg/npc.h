#pragma once
#include <string>
#include <functional>
#include "../graphics/animated_sprite.h"

namespace MoLin::RPG {

class NPC : public AnimatedSprite {
public:
    NPC(const std::string& name, SDL_Texture* texture);

    void SetDialogue(const std::string& text) { m_Dialogue = text; }
    std::string GetDialogue() const { return m_Dialogue; }

    // 交互回调（由游戏逻辑设置）
    std::function<void()> onInteract;

protected:
    std::string m_Dialogue;
};

} // namespace MoLin::RPG
