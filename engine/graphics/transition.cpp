#include "transition.h"

namespace MoLin {

Transition::Transition() {}

void Transition::StartFadeIn(float duration) {
    m_Type = FadeIn;
    m_Duration = duration;
    m_Elapsed = 0.0f;
    m_Alpha = 255;
}

void Transition::StartFadeOut(float duration) {
    m_Type = FadeOut;
    m_Duration = duration;
    m_Elapsed = 0.0f;
    m_Alpha = 0;
}

void Transition::Update(float delta) {
    if (m_Type == None) return;
    m_Elapsed += delta;
    float progress = m_Elapsed / m_Duration;
    if (progress >= 1.0f) {
        progress = 1.0f;
        m_Type = None;
    }
    if (m_Type == FadeIn) m_Alpha = static_cast<Uint8>(255 * (1.0f - progress));
    else if (m_Type == FadeOut) m_Alpha = static_cast<Uint8>(255 * progress);
}

void Transition::Render(SDL_Renderer* renderer) {
    if (m_Type == None) return;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, m_Alpha);
    SDL_Rect full = {0, 0, 1920, 1080}; // 覆盖全屏，实际应从引擎获取
    SDL_RenderFillRect(renderer, &full);
}

} // namespace MoLin
