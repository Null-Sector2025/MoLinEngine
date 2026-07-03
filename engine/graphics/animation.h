#pragma once
#include <SDL2/SDL.h>
#include <vector>

namespace MoLin {

struct AnimationFrame {
    SDL_Rect clip;
    float duration; // 秒
};

class Animation {
public:
    Animation();
    void AddFrame(const SDL_Rect& clip, float duration);
    void Update(float delta);
    SDL_Rect GetCurrentFrame() const;
    void Reset();
    void SetLooping(bool loop) { m_Looping = loop; }

private:
    std::vector<AnimationFrame> m_Frames;
    int m_CurrentFrame = 0;
    float m_Elapsed = 0.0f;
    bool m_Looping = true;
    bool m_Finished = false;
};

} // namespace MoLin
