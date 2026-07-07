#pragma once
#include "sprite.h"
#include "animation.h"
#include <unordered_map>

namespace MoLin {

class AnimatedSprite : public Sprite {
public:
    AnimatedSprite(const std::string& name, SDL_Texture* texture);

    void AddAnimation(const std::string& animName, const Animation& anim);
    void PlayAnimation(const std::string& animName);
    void Pause();
    void Resume();

protected:
    void OnUpdate(float delta) override;

    std::unordered_map<std::string, Animation> m_Animations;
    Animation* m_CurrentAnim = nullptr;
    std::string m_CurrentAnimName;
    bool m_Paused = false;
};

} // namespace MoLin
