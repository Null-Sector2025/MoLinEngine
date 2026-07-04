#include "collision.h"
#include <cmath>

namespace MoLin {

bool Collision::RectRect(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b);
}

bool Collision::PointRect(int x, int y, const SDL_Rect& rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

bool Collision::CircleCircle(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int distSq = dx * dx + dy * dy;
    int radiusSum = r1 + r2;
    return distSq <= radiusSum * radiusSum;
}

bool Collision::RectCircle(const SDL_Rect& rect, int cx, int cy, int radius) {
    int closestX = std::max(rect.x, std::min(cx, rect.x + rect.w));
    int closestY = std::max(rect.y, std::min(cy, rect.y + rect.h));
    int dx = cx - closestX;
    int dy = cy - closestY;
    return (dx * dx + dy * dy) <= (radius * radius);
}

} // namespace MoLin
