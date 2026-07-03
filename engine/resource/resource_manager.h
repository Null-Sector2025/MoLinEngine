#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace MoLin {

class ResourceManager {
public:
    static ResourceManager& Instance() {
        static ResourceManager instance;
        return instance;
    }

    SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer);
    Mix_Chunk* LoadSound(const std::string& path);
    Mix_Music* LoadMusic(const std::string& path);

    void UnloadTexture(const std::string& path);
    void UnloadSound(const std::string& path);
    void UnloadMusic(const std::string& path);

    void ClearAll();

private:
    ResourceManager() = default;
    ~ResourceManager() { ClearAll(); }
    std::unordered_map<std::string, SDL_Texture*> m_Textures;
    std::unordered_map<std::string, Mix_Chunk*> m_Sounds;
    std::unordered_map<std::string, Mix_Music*> m_Musics;
};

} // namespace MoLin
