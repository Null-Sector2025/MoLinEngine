#pragma once
#include <SDL2/SDL.h>

namespace MoLin::Collision {

inline bool PointInRect(int px, int py, const SDL_Rect& rect) {
    return (px >= rect.x && px <= rect.x + rect.w &&
            py >= rect.y && py <= rect.y + rect.h);
}

inline bool RectIntersect(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y);
}

} // namespace MoLin::Collision
