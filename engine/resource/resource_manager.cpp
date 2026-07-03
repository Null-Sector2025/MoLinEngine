#include "resource_manager.h"

namespace MoLin {

SDL_Texture* ResourceManager::LoadTexture(const std::string& path, SDL_Renderer* renderer) {
    auto it = m_Textures.find(path);
    if (it != m_Textures.end()) return it->second;

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image " << path << ": " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture) {
        m_Textures[path] = texture;
        std::cout << "[Resource] Loaded texture: " << path << std::endl;
    }
    return texture;
}

Mix_Chunk* ResourceManager::LoadSound(const std::string& path) {
    auto it = m_Sounds.find(path);
    if (it != m_Sounds.end()) return it->second;

    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (chunk) {
        m_Sounds[path] = chunk;
        std::cout << "[Resource] Loaded sound: " << path << std::endl;
    } else {
        std::cerr << "Failed to load sound " << path << ": " << Mix_GetError() << std::endl;
    }
    return chunk;
}

Mix_Music* ResourceManager::LoadMusic(const std::string& path) {
    auto it = m_Musics.find(path);
    if (it != m_Musics.end()) return it->second;

    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (music) {
        m_Musics[path] = music;
        std::cout << "[Resource] Loaded music: " << path << std::endl;
    } else {
        std::cerr << "Failed to load music " << path << ": " << Mix_GetError() << std::endl;
    }
    return music;
}

void ResourceManager::UnloadTexture(const std::string& path) {
    auto it = m_Textures.find(path);
    if (it != m_Textures.end()) {
        SDL_DestroyTexture(it->second);
        m_Textures.erase(it);
    }
}

void ResourceManager::UnloadSound(const std::string& path) {
    auto it = m_Sounds.find(path);
    if (it != m_Sounds.end()) {
        Mix_FreeChunk(it->second);
        m_Sounds.erase(it);
    }
}

void ResourceManager::UnloadMusic(const std::string& path) {
    auto it = m_Musics.find(path);
    if (it != m_Musics.end()) {
        Mix_FreeMusic(it->second);
        m_Musics.erase(it);
    }
}

void ResourceManager::ClearAll() {
    for (auto& p : m_Textures) SDL_DestroyTexture(p.second);
    for (auto& p : m_Sounds) Mix_FreeChunk(p.second);
    for (auto& p : m_Musics) Mix_FreeMusic(p.second);
    m_Textures.clear();
    m_Sounds.clear();
    m_Musics.clear();
}

} // namespace MoLin
