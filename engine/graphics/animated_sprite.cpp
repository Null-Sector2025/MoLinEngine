#include "animated_sprite.h"

namespace MoLin {

AnimatedSprite::AnimatedSprite(const std::string& name, SDL_Texture* tex) : Sprite(name, tex) {}
void AnimatedSprite::AddAnimation(const std::string& name, const Animation& anim) { m_Animations[name] = anim; }
void AnimatedSprite::PlayAnimation(const std::string& name) {
    auto it = m_Animations.find(name);
    if (it != m_Animations.end()) { m_CurrentAnim = &it->second; m_CurrentAnim->Reset(); m_Paused = false; }
}
void AnimatedSprite::OnUpdate(float delta) {
    if (m_CurrentAnim && !m_Paused) {
        m_CurrentAnim->Update(delta);
        SetSourceRect(m_CurrentAnim->GetCurrentFrame());
    }
}

} // namespace MoLin
