#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>

namespace MoLin {

class TextRenderer {
public:
    static TextRenderer& Instance() {
        static TextRenderer instance;
        return instance;
    }

    bool Initialize();
    void Shutdown();

    TTF_Font* LoadFont(const std::string& path, int size);
    SDL_Texture* RenderText(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);
    void RenderTextDirect(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);

private:
    TextRenderer() = default;
    std::unordered_map<std::string, TTF_Font*> m_Fonts;
};

} // namespace MoLin
