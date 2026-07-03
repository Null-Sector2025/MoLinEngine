#include "text_renderer.h"
#include <iostream>

namespace MoLin {

TextRenderer::TextRenderer() {
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf init failed: " << TTF_GetError() << std::endl;
    }
}

TextRenderer::~TextRenderer() {
    if (m_Font) {
        TTF_CloseFont(m_Font);
    }
    TTF_Quit();
}

bool TextRenderer::LoadFont(const std::string& path, int size) {
    if (m_Font) TTF_CloseFont(m_Font);
    m_Font = TTF_OpenFont(path.c_str(), size);
    if (!m_Font) {
        std::cerr << "Failed to load font " << path << ": " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

SDL_Texture* TextRenderer::RenderText(const std::string& text, SDL_Color color, SDL_Renderer* renderer) {
    if (!m_Font) return nullptr;
    SDL_Surface* surface = TTF_RenderUTF8_Solid(m_Font, text.c_str(), color);
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

} // namespace MoLin
