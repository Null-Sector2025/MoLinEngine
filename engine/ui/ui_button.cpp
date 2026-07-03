#include "ui_button.h"

namespace MoLin {

UIButton::UIButton(const std::string& name, SDL_Texture* normal, SDL_Texture* hover)
    : SceneNode(name), m_NormalTex(normal), m_HoverTex(hover) {}
void UIButton::OnRender(SDL_Renderer* renderer) {
    SDL_Texture* tex = (m_Hovered && m_HoverTex) ? m_HoverTex : m_NormalTex;
    if (!tex) return;
    Transform world = GetWorldTransform();
    SDL_Rect dest;
    SDL_QueryTexture(tex, nullptr, nullptr, &dest.w, &dest.h);
    dest.x = static_cast<int>(world.x); dest.y = static_cast<int>(world.y);
    SDL_RenderCopy(renderer, tex, nullptr, &dest);
}
bool UIButton::CheckClick(int mx, int my) {
    Transform world = GetWorldTransform();
    SDL_Rect rect;
    SDL_QueryTexture(m_NormalTex, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = static_cast<int>(world.x); rect.y = static_cast<int>(world.y);
    if (mx >= rect.x && mx <= rect.x+rect.w && my >= rect.y && my <= rect.y+rect.h) {
        if (m_OnClick) m_OnClick();
        return true;
    }
    return false;
}

} // namespace MoLin
