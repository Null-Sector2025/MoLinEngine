#include "text_renderer.h"
#include <iostream>

namespace MoLin {

bool TextRenderer::Initialize() {
    if (TTF_Init() == -1) {
        std::cerr << "[Text] TTF_Init failed: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void TextRenderer::Shutdown() {
    for (auto& [key, font] : m_Fonts) {
        TTF_CloseFont(font);
    }
    m_Fonts.clear();
    TTF_Quit();
}

TTF_Font* TextRenderer::LoadFont(const std::string& path, int size) {
    std::string key = path + ":" + std::to_string(size);
    auto it = m_Fonts.find(key);
    if (it != m_Fonts.end()) return it->second;

    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (!font) {
        std::cerr << "[Text] Failed to load font: " << path << std::endl;
        return nullptr;
    }
    m_Fonts[key] = font;
    return font;
}

SDL_Texture* TextRenderer::RenderText(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    if (!font || text.empty()) return nullptr;
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void TextRenderer::RenderTextDirect(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Texture* tex = RenderText(text, font, color, renderer);
    if (!tex) return;
    SDL_Rect dst = {x, y, 0, 0};
    SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}

} // namespace MoLin
