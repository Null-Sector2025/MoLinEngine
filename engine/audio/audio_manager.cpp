#include "audio/audio_manager.h"
#include <iostream>

namespace MoLin {

AudioManager::AudioManager() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer 初始化失败: " << Mix_GetError() << std::endl;
    }
}

AudioManager::~AudioManager() {
    // 释放所有音效
    for (auto& pair : m_sounds) {
        Mix_FreeChunk(pair.second);
    }
    m_sounds.clear();
    
    // 释放所有音乐
    for (auto& pair : m_music) {
        Mix_FreeMusic(pair.second);
    }
    m_music.clear();
    
    Mix_CloseAudio();
}

bool AudioManager::LoadSound(const std::string& name, const std::string& filepath) {
    Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str());
    if (!chunk) {
        std::cerr << "加载音效失败: " << filepath << " - " << Mix_GetError() << std::endl;
        return false;
    }
    m_sounds[name] = chunk;
    return true;
}

bool AudioManager::LoadMusic(const std::string& name, const std::string& filepath) {
    Mix_Music* music = Mix_LoadMUS(filepath.c_str());
    if (!music) {
        std::cerr << "加载音乐失败: " << filepath << " - " << Mix_GetError() << std::endl;
        return false;
    }
    m_music[name] = music;
    return true;
}

void AudioManager::PlaySound(const std::string& name, float volume) {
    auto it = m_sounds.find(name);
    if (it == m_sounds.end()) {
        std::cerr << "音效未找到: " << name << std::endl;
        return;
    }
    int vol = static_cast<int>(volume * m_soundVolume);
    Mix_VolumeChunk(it->second, vol);
    Mix_PlayChannel(-1, it->second, 0);
}

void AudioManager::PlayMusic(const std::string& name, float volume, bool loop) {
    auto it = m_music.find(name);
    if (it == m_music.end()) {
        std::cerr << "音乐未找到: " << name << std::endl;
        return;
    }
    m_currentMusic = it->second;
    int loops = loop ? -1 : 0;
    Mix_PlayMusic(m_currentMusic, loops);
    int vol = static_cast<int>(volume * m_musicVolume);
    Mix_VolumeMusic(vol);
}

void AudioManager::StopMusic() {
    Mix_HaltMusic();
}

void AudioManager::PauseMusic() {
    Mix_PauseMusic();
}

void AudioManager::ResumeMusic() {
    Mix_ResumeMusic();
}

void AudioManager::SetVolume(int volume) {
    m_volume = std::max(0, std::min(128, volume));
    // 应用到所有音效和音乐
    SetSoundVolume(m_volume);
    SetMusicVolume(m_volume);
}

void AudioManager::SetMusicVolume(int volume) {
    m_musicVolume = std::max(0, std::min(128, volume));
    Mix_VolumeMusic(m_musicVolume);
}

void AudioManager::SetSoundVolume(int volume) {
    m_soundVolume = std::max(0, std::min(128, volume));
    // 应用到所有已加载的音效
    for (auto& pair : m_sounds) {
        Mix_VolumeChunk(pair.second, m_soundVolume);
    }
}

} // namespace MoLin
