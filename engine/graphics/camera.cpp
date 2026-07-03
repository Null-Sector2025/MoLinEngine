#include "camera.h"

namespace MoLin {

Camera::Camera(int viewportWidth, int viewportHeight)
    : m_ViewW(viewportWidth), m_ViewH(viewportHeight) {}

int Camera::WorldToScreenX(float worldX) const {
    return static_cast<int>((worldX - m_X) * m_Zoom + m_ViewW / 2.0f);
}
int Camera::WorldToScreenY(float worldY) const {
    return static_cast<int>((worldY - m_Y) * m_Zoom + m_ViewH / 2.0f);
}
float Camera::ScreenToWorldX(int screenX) const {
    return (screenX - m_ViewW / 2.0f) / m_Zoom + m_X;
}
float Camera::ScreenToWorldY(int screenY) const {
    return (screenY - m_ViewH / 2.0f) / m_Zoom + m_Y;
}

SDL_Rect Camera::ApplyToRect(const SDL_Rect& worldRect) const {
    SDL_Rect result;
    result.x = WorldToScreenX(worldRect.x);
    result.y = WorldToScreenY(worldRect.y);
    result.w = static_cast<int>(worldRect.w * m_Zoom);
    result.h = static_cast<int>(worldRect.h * m_Zoom);
    return result;
}

} // namespace MoLin
