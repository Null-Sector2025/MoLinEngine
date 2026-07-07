#include "ui_system.h"
#include <iostream>

namespace MoLin::UI {

UILabel::UILabel(const std::string& name, TTF_Font* font)
    : SceneNode(name), m_Font(font) {}

void UILabel::OnRender(SDL_Renderer* renderer) {
    if (m_Text.empty() || !m_Font) return;
    Transform world = GetWorldTransform();
    TextRenderer::Instance().RenderTextDirect(
        m_Text, static_cast<int>(world.x), static_cast<int>(world.y),
        m_Font, m_Color, renderer);
}

UIProgressBar::UIProgressBar(const std::string& name, int width, int height)
    : SceneNode(name), m_BarWidth(width), m_BarHeight(height) {}

void UIProgressBar::SetProgress(float progress) {
    m_Progress = std::max(0.0f, std::min(1.0f, progress));
}

void UIProgressBar::SetColors(SDL_Color bg, SDL_Color fill) {
    m_BgColor = bg;
    m_FillColor = fill;
}

void UIProgressBar::OnRender(SDL_Renderer* renderer) {
    Transform world = GetWorldTransform();
    SDL_Rect bgRect = {static_cast<int>(world.x), static_cast<int>(world.y), m_BarWidth, m_BarHeight};
    SDL_SetRenderDrawColor(renderer, m_BgColor.r, m_BgColor.g, m_BgColor.b, m_BgColor.a);
    SDL_RenderFillRect(renderer, &bgRect);

    SDL_Rect fillRect = {static_cast<int>(world.x), static_cast<int>(world.y),
                         static_cast<int>(m_BarWidth * m_Progress), m_BarHeight};
    SDL_SetRenderDrawColor(renderer, m_FillColor.r, m_FillColor.g, m_FillColor.b, m_FillColor.a);
    SDL_RenderFillRect(renderer, &fillRect);
}

UIPanel::UIPanel(const std::string& name, int width, int height)
    : SceneNode(name), m_PanelWidth(width), m_PanelHeight(height) {}

void UIPanel::OnRender(SDL_Renderer* renderer) {
    Transform world = GetWorldTransform();
    SDL_Rect rect = {static_cast<int>(world.x), static_cast<int>(world.y), m_PanelWidth, m_PanelHeight};
    SDL_SetRenderDrawColor(renderer, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
    SDL_RenderFillRect(renderer, &rect);

    if (m_DrawBorder) {
        SDL_SetRenderDrawColor(renderer, m_BorderColor.r, m_BorderColor.g, m_BorderColor.b, m_BorderColor.a);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

} // namespace MoLin::UI
