#include "animation.h"

namespace MoLin {

Animation::Animation() {}

void Animation::AddFrame(const SDL_Rect& clip, float duration) {
    m_Frames.push_back({clip, duration});
}

void Animation::Update(float delta) {
    if (m_Finished || m_Frames.empty()) return;
    m_Elapsed += delta;
    if (m_Elapsed >= m_Frames[m_CurrentFrame].duration) {
        m_Elapsed = 0.0f;
        m_CurrentFrame++;
        if (m_CurrentFrame >= m_Frames.size()) {
            if (m_Looping) {
                m_CurrentFrame = 0;
            } else {
                m_CurrentFrame = m_Frames.size() - 1;
                m_Finished = true;
            }
        }
    }
}

SDL_Rect Animation::GetCurrentFrame() const {
    if (m_Frames.empty()) return {0,0,0,0};
    return m_Frames[m_CurrentFrame].clip;
}

void Animation::Reset() {
    m_CurrentFrame = 0;
    m_Elapsed = 0.0f;
    m_Finished = false;
}

} // namespace MoLin
