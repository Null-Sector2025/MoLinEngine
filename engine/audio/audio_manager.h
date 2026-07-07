#pragma once
#include <string>
#include <unordered_map>
#include <SDL2/SDL_mixer.h>

namespace MoLin {

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // 音效播放
    void PlaySound(const std::string& name, float volume = 1.0f);
    void PlayMusic(const std::string& name, float volume = 1.0f, bool loop = true);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    void SetVolume(int volume);
    void SetMusicVolume(int volume);
    void SetSoundVolume(int volume);

    // 加载音频文件
    bool LoadSound(const std::string& name, const std::string& filepath);
    bool LoadMusic(const std::string& name, const std::string& filepath);

private:
    std::unordered_map<std::string, Mix_Chunk*> m_sounds;
    std::unordered_map<std::string, Mix_Music*> m_music;
    Mix_Music* m_currentMusic = nullptr;
    int m_volume = 128;
    int m_musicVolume = 128;
    int m_soundVolume = 128;
};

} // namespace MoLin
