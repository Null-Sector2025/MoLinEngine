#include "audio_manager.h"
#include <iostream>
#include <cmath>

namespace MoLin {

AudioManager& AudioManager::Instance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() {}
AudioManager::~AudioManager() { UnloadAll(); }

void AudioManager::SetMasterVolume(float volume) {
    m_MasterVolume = std::max(0.0f, std::min(1.0f, volume));
    Mix_Volume(-1, static_cast<int>(m_MasterVolume * 128));
}

float AudioManager::GetMasterVolume() const { return m_MasterVolume; }

void AudioManager::LoadSound(const std::string& id, const std::string& path) {
    auto* chunk = Mix_LoadWAV(path.c_str());
    if (chunk) {
        m_Sounds[id] = chunk;
    } else {
        std::cerr << "[Audio] Failed to load sound: " << path << " - " << Mix_GetError() << std::endl;
    }
}

void AudioManager::LoadMusic(const std::string& id, const std::string& path) {
    auto* music = Mix_LoadMUS(path.c_str());
    if (music) {
        m_Musics[id] = music;
    } else {
        std::cerr << "[Audio] Failed to load music: " << path << " - " << Mix_GetError() << std::endl;
    }
}

int AudioManager::PlaySound(const std::string& id, float volume, bool loop) {
    auto it = m_Sounds.find(id);
    if (it == m_Sounds.end()) return -1;
    int channel = Mix_PlayChannel(-1, it->second, loop ? -1 : 0);
    if (channel >= 0) {
        Mix_Volume(channel, static_cast<int>(volume * m_MasterVolume * 128));
    }
    return channel;
}

void AudioManager::StopSound(int channel) {
    if (channel >= 0) Mix_HaltChannel(channel);
}

void AudioManager::FadeOutChannel(int channel, int ms) {
    if (channel >= 0) Mix_FadeOutChannel(channel, ms);
}

void AudioManager::FadeInChannel(int channel, int ms) {
    if (channel >= 0) Mix_FadeInChannel(channel, ms, -1);
}

void AudioManager::PlayMusic(const std::string& id, bool loop, int fadeInMs) {
    auto it = m_Musics.find(id);
    if (it == m_Musics.end()) return;
    m_CurrentMusicId = id;
    if (fadeInMs > 0) {
        Mix_FadeInMusic(it->second, loop ? -1 : 1, fadeInMs);
    } else {
        Mix_PlayMusic(it->second, loop ? -1 : 1);
    }
    Mix_VolumeMusic(static_cast<int>(m_MusicVolume * 128));
}

void AudioManager::StopMusic(int fadeOutMs) {
    if (fadeOutMs > 0) {
        Mix_FadeOutMusic(fadeOutMs);
    } else {
        Mix_HaltMusic();
    }
    m_CurrentMusicId.clear();
}

void AudioManager::PauseMusic() { Mix_PauseMusic(); }
void AudioManager::ResumeMusic() { Mix_ResumeMusic(); }

void AudioManager::SetMusicVolume(float volume) {
    m_MusicVolume = std::max(0.0f, std::min(1.0f, volume));
    Mix_VolumeMusic(static_cast<int>(m_MusicVolume * 128));
}

bool AudioManager::IsMusicPlaying() const {
    return Mix_PlayingMusic() == 1;
}

void AudioManager::UnloadSound(const std::string& id) {
    auto it = m_Sounds.find(id);
    if (it != m_Sounds.end()) {
        Mix_FreeChunk(it->second);
        m_Sounds.erase(it);
    }
}

void AudioManager::UnloadAll() {
    for (auto& [id, chunk] : m_Sounds) Mix_FreeChunk(chunk);
    for (auto& [id, music] : m_Musics) Mix_FreeMusic(music);
    m_Sounds.clear();
    m_Musics.clear();
}

void AudioManager::SetListenerPosition(float x, float y) {
    m_ListenerX = x;
    m_ListenerY = y;
}

void AudioManager::PlaySound3D(const std::string& id, float sourceX, float sourceY, float maxDistance) {
    auto it = m_Sounds.find(id);
    if (it == m_Sounds.end()) return;

    float dx = sourceX - m_ListenerX;
    float dy = sourceY - m_ListenerY;
    float dist = std::sqrt(dx * dx + dy * dy);
    float volume = 1.0f - std::min(1.0f, dist / maxDistance);
    if (volume <= 0.0f) return;

    int channel = Mix_PlayChannel(-1, it->second, 0);
    if (channel >= 0) {
        int vol = static_cast<int>(volume * m_MasterVolume * 128);
        Mix_Volume(channel, vol);

        // 简单的左右声道平衡
        float pan = dx / maxDistance;
        pan = std::max(-1.0f, std::min(1.0f, pan));
        Uint8 left = static_cast<Uint8>(255 * (1.0f - std::max(0.0f, pan)));
        Uint8 right = static_cast<Uint8>(255 * (1.0f + std::min(0.0f, pan)));
        Mix_SetPanning(channel, left, right);
    }
}

} // namespace MoLin
