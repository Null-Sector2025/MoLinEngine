#pragma once
#include "../scene/scene.h"
#include "graphics/text_renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include <vector>

namespace MoLin::UI {

class UILabel : public SceneNode {
public:
    UILabel(const std::string& name, TTF_Font* font);
    void SetText(const std::string& text) { m_Text = text; }
    void SetColor(SDL_Color color) { m_Color = color; }
    void SetFont(TTF_Font* font) { m_Font = font; }
    const std::string& GetText() const { return m_Text; }
protected:
    void OnRender(SDL_Renderer* renderer) override;
    std::string m_Text;
    TTF_Font* m_Font;
    SDL_Color m_Color = {255, 255, 255, 255};
};

class UIProgressBar : public SceneNode {
public:
    UIProgressBar(const std::string& name, int width, int height);
    void SetProgress(float progress);
    float GetProgress() const { return m_Progress; }
    void SetColors(SDL_Color bg, SDL_Color fill);
protected:
    void OnRender(SDL_Renderer* renderer) override;
    float m_Progress = 0.0f;
    int m_BarWidth, m_BarHeight;
    SDL_Color m_BgColor = {60, 60, 60, 255};
    SDL_Color m_FillColor = {0, 255, 0, 255};
};

class UIPanel : public SceneNode {
public:
    UIPanel(const std::string& name, int width, int height);
    void SetColor(SDL_Color color) { m_Color = color; }
    void SetBorderColor(SDL_Color color) { m_BorderColor = color; m_DrawBorder = true; }
protected:
    void OnRender(SDL_Renderer* renderer) override;
    int m_PanelWidth, m_PanelHeight;
    SDL_Color m_Color = {30, 30, 30, 200};
    SDL_Color m_BorderColor = {100, 100, 100, 255};
    bool m_DrawBorder = false;
};

} // namespace MoLin::UI
