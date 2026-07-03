#include "ui_button.h"

namespace MoLin {

UIButton::UIButton(const std::string& name, SDL_Texture* normal, SDL_Texture* hover)
    : SceneNode(name), m_NormalTex(normal), m_HoverTex(hover) {}

void UIButton::OnUpdate(float delta) {
    // 鼠标检测在外部通过 CheckClick 触发，这里可做状态更新
}

void UIButton::OnRender(SDL_Renderer* renderer) {
    SDL_Texture* tex = (m_Hovered && m_HoverTex) ? m_HoverTex : m_NormalTex;
    if (!tex) return;
    Transform world = GetWorldTransform();
    SDL_Rect destRect;
    SDL_QueryTexture(tex, nullptr, nullptr, &destRect.w, &destRect.h);
    destRect.x = static_cast<int>(world.x);
    destRect.y = static_cast<int>(world.y);
    SDL_RenderCopy(renderer, tex, nullptr, &destRect);
}

bool UIButton::CheckClick(int mouseX, int mouseY) {
    Transform world = GetWorldTransform();
    SDL_Rect rect;
    SDL_QueryTexture(m_NormalTex, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = static_cast<int>(world.x);
    rect.y = static_cast<int>(world.y);
    if (mouseX >= rect.x && mouseX <= rect.x+rect.w &&
        mouseY >= rect.y && mouseY <= rect.y+rect.h) {
        if (m_OnClick) m_OnClick();
        return true;
    }
    return false;
}

} // namespace MoLin
