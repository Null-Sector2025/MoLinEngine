#include "animated_sprite.h"

namespace MoLin {

AnimatedSprite::AnimatedSprite(const std::string& name, SDL_Texture* texture)
    : Sprite(name, texture) {}

void AnimatedSprite::AddAnimation(const std::string& animName, const Animation& anim) {
    m_Animations[animName] = anim;
}

void AnimatedSprite::PlayAnimation(const std::string& animName) {
    auto it = m_Animations.find(animName);
    if (it != m_Animations.end()) {
        m_CurrentAnim = &it->second;
        m_CurrentAnimName = animName;
        m_CurrentAnim->Reset();
        m_Paused = false;
    }
}

void AnimatedSprite::Pause() { m_Paused = true; }
void AnimatedSprite::Resume() { m_Paused = false; }

void AnimatedSprite::OnUpdate(float delta) {
    if (m_CurrentAnim && !m_Paused) {
        m_CurrentAnim->Update(delta);
        SetSourceRect(m_CurrentAnim->GetCurrentFrame());
    }
}

} // namespace MoLin
