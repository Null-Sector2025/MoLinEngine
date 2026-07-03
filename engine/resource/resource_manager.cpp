#include "resource_manager.h"
#include <iostream>

namespace MoLin {

SDL_Texture* ResourceManager::LoadTexture(const std::string& path, SDL_Renderer* renderer) {
    auto it = m_Textures.find(path);
    if (it != m_Textures.end()) return it->second;
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture) m_Textures[path] = texture;
    return texture;
}
Mix_Chunk* ResourceManager::LoadSound(const std::string& path) {
    auto it = m_Sounds.find(path);
    if (it != m_Sounds.end()) return it->second;
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (chunk) m_Sounds[path] = chunk;
    return chunk;
}
Mix_Music* ResourceManager::LoadMusic(const std::string& path) {
    auto it = m_Musics.find(path);
    if (it != m_Musics.end()) return it->second;
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (music) m_Musics[path] = music;
    return music;
}
void ResourceManager::ClearAll() {
    for (auto& p : m_Textures) SDL_DestroyTexture(p.second);
    for (auto& p : m_Sounds) Mix_FreeChunk(p.second);
    for (auto& p : m_Musics) Mix_FreeMusic(p.second);
    m_Textures.clear(); m_Sounds.clear(); m_Musics.clear();
}

} // namespace MoLin
