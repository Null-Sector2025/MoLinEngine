#pragma once
#include "scene/scene.h"
#include <SDL2/SDL.h>

namespace MoLin {

enum class AnchorPoint {
    TopLeft, TopCenter, TopRight,
    CenterLeft, Center, CenterRight,
    BottomLeft, BottomCenter, BottomRight
};

class Sprite : public SceneNode {
public:
    Sprite(const std::string& name, SDL_Texture* texture = nullptr);

    void SetTexture(SDL_Texture* texture);
    SDL_Texture* GetTexture() const { return m_Texture; }

    void SetSourceRect(const SDL_Rect& rect) { m_SrcRect = rect; }
    const SDL_Rect& GetSourceRect() const { return m_SrcRect; }

    void SetFlip(SDL_RendererFlip flip) { m_Flip = flip; }
    void SetAnchor(AnchorPoint anchor) { m_Anchor = anchor; }
    void SetAlpha(Uint8 alpha) { m_Alpha = alpha; }
    void SetColorMod(Uint8 r, Uint8 g, Uint8 b) { m_ColorMod = {r, g, b}; m_UseColorMod = true; }
    void ClearColorMod() { m_UseColorMod = false; }

    int GetWidth() const;
    int GetHeight() const;

protected:
    void OnRender(SDL_Renderer* renderer) override;

    SDL_Texture* m_Texture = nullptr;
    SDL_Rect m_SrcRect = {0, 0, 0, 0};
    SDL_RendererFlip m_Flip = SDL_FLIP_NONE;
    AnchorPoint m_Anchor = AnchorPoint::TopLeft;
    Uint8 m_Alpha = 255;
    SDL_Color m_ColorMod = {255, 255, 255};
    bool m_UseColorMod = false;

    void ApplyAnchor(SDL_Rect& dst) const;
};

} // namespace MoLin
