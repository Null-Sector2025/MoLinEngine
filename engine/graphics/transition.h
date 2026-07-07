#pragma once
#include <SDL2/SDL.h>

namespace MoLin {

class Transition {
public:
    enum Type { FadeIn, FadeOut, None };

    Transition();
    void StartFadeIn(float duration);
    void StartFadeOut(float duration);
    void Update(float delta);
    void Render(SDL_Renderer* renderer);
    bool IsFinished() const { return m_Type == None; }
    Type GetType() const { return m_Type; }

private:
    Type m_Type = None;
    float m_Duration = 0.0f;
    float m_Elapsed = 0.0f;
    Uint8 m_Alpha = 0;
};

} // namespace MoLin
