#pragma once
#include "../scene/scene.h"
#include <SDL2/SDL.h>

namespace MoLin {

class Sprite : public SceneNode {
public:
    Sprite(const std::string& name, SDL_Texture* texture = nullptr);
    void SetTexture(SDL_Texture* texture);
    void SetSourceRect(const SDL_Rect& rect) { m_SrcRect = rect; }
    void SetFlip(SDL_RendererFlip flip) { m_Flip = flip; }

protected:
    void OnRender(SDL_Renderer* renderer) override;

    SDL_Texture* m_Texture = nullptr;
    SDL_Rect m_SrcRect = {0,0,0,0};
    SDL_RendererFlip m_Flip = SDL_FLIP_NONE;
};

} // namespace MoLin
