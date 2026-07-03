#pragma once
#include <SDL2/SDL_ttf.h>
#include <string>
#include <memory>

namespace MoLin {

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    bool LoadFont(const std::string& path, int size);
    SDL_Texture* RenderText(const std::string& text, SDL_Color color, SDL_Renderer* renderer);

private:
    TTF_Font* m_Font = nullptr;
};

} // namespace MoLin
