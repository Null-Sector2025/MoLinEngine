#include "audio/audio_manager.h"
#include <SDL2/SDL_mixer.h>
#include <iostream>

namespace MoLin {

AudioManager::AudioManager() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer 初始化失败: " << Mix_GetError() << std::endl;
    }
}

AudioManager::~AudioManager() {
    Mix_CloseAudio();
}

void AudioManager::PlaySound(const std::string& name, float volume) {
    // TODO: 实现音效播放
    std::cout << "播放音效: " << name << " (音量: " << volume << ")" << std::endl;
}

void AudioManager::PlayMusic(const std::string& name, float volume, bool loop) {
    // TODO: 实现音乐播放
    std::cout << "播放音乐: " << name << " (音量: " << volume << ", 循环: " << loop << ")" << std::endl;
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
    Mix_VolumeMusic(volume);
}

} // namespace MoLin
