#pragma once
#include <SDL2/SDL.h>

namespace MoLin {

class Camera {
public:
    Camera(int viewportWidth, int viewportHeight);

    void SetPosition(float x, float y) { m_X = x; m_Y = y; }
    void Move(float dx, float dy) { m_X += dx; m_Y += dy; }
    void SetZoom(float zoom) { m_Zoom = (zoom > 0.01f) ? zoom : 0.01f; }

    // 世界坐标 → 屏幕坐标
    int WorldToScreenX(float worldX) const;
    int WorldToScreenY(float worldY) const;
    // 屏幕坐标 → 世界坐标
    float ScreenToWorldX(int screenX) const;
    float ScreenToWorldY(int screenY) const;

    SDL_Rect ApplyToRect(const SDL_Rect& worldRect) const;

private:
    int m_ViewW, m_ViewH;
    float m_X = 0, m_Y = 0;
    float m_Zoom = 1.0f;
};

} // namespace MoLin
