#include "camera.h"

namespace MoLin {

Camera::Camera(int vw, int vh) : m_ViewW(vw), m_ViewH(vh) {}
int Camera::WorldToScreenX(float wx) const { return static_cast<int>((wx - m_X) * m_Zoom + m_ViewW / 2.0f); }
int Camera::WorldToScreenY(float wy) const { return static_cast<int>((wy - m_Y) * m_Zoom + m_ViewH / 2.0f); }
float Camera::ScreenToWorldX(int sx) const { return (sx - m_ViewW / 2.0f) / m_Zoom + m_X; }
float Camera::ScreenToWorldY(int sy) const { return (sy - m_ViewH / 2.0f) / m_Zoom + m_Y; }

} // namespace MoLin
