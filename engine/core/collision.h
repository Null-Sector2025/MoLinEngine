#pragma once
#include <SDL2/SDL.h>

namespace MoLin {

class Collision {
public:
    static bool RectRect(const SDL_Rect& a, const SDL_Rect& b);
    static bool PointRect(int x, int y, const SDL_Rect& rect);
    static bool CircleCircle(int x1, int y1, int r1, int x2, int y2, int r2);
    static bool RectCircle(const SDL_Rect& rect, int cx, int cy, int radius);
};

} // namespace MoLin
