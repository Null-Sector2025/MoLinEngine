#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "scene/scene.h"

namespace MoLin {

struct SpriteBatchItem {
    SDL_Texture* texture = nullptr;
    SDL_Rect src = {0,0,0,0};
    SDL_Rect dst = {0,0,0,0};
    float rotation = 0.0f;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int layer = 0;
};

class SpriteBatch {
public:
    SpriteBatch(SDL_Renderer* renderer);
    void AddItem(const SpriteBatchItem& item);
    void Render();
    void Clear();
    void SetCameraOffset(float x, float y) { m_CamX = x; m_CamY = y; }

private:
    SDL_Renderer* m_Renderer;
    std::vector<SpriteBatchItem> m_Items;
    float m_CamX = 0.0f, m_CamY = 0.0f;
    void SortByLayer();
};

} // namespace MoLin
