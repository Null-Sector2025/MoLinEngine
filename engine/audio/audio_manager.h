#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <queue>
#include <functional>

namespace MoLin {

struct AudioSource {
    Mix_Chunk* chunk = nullptr;
    int channel = -1;
    float volume = 1.0f;
    bool loop = false;
};

class AudioManager {
public:
    static AudioManager& Instance();
    ~AudioManager();

    void SetMasterVolume(float volume);
    float GetMasterVolume() const { return m_MasterVolume; }

    int PlaySound(const std::string& id, float volume = 1.0f, bool loop = false);
    void StopSound(int channel);
    void FadeOutChannel(int channel, int ms);
    void FadeInChannel(int channel, int ms);

    void PlayMusic(const std::string& id, bool loop = true, int fadeInMs = 0);
    void StopMusic(int fadeOutMs = 0);
    void PauseMusic();
    void ResumeMusic();
    void SetMusicVolume(float volume);
    bool IsMusicPlaying() const;

    void LoadSound(const std::string& id, const std::string& path);
    void LoadMusic(const std::string& id, const std::string& path);
    void UnloadSound(const std::string& id);
    void UnloadAll();

    // 模拟3D音效：根据听者和声源位置计算左右声道和距离衰减
    void SetListenerPosition(float x, float y);
    void PlaySound3D(const std::string& id, float sourceX, float sourceY, float maxDistance = 500.0f);

private:
    AudioManager();
    float m_MasterVolume = 1.0f;
    float m_MusicVolume = 1.0f;
    float m_ListenerX = 0.0f;
    float m_ListenerY = 0.0f;
    std::string m_CurrentMusicId;
    std::unordered_map<std::string, Mix_Chunk*> m_Sounds;
    std::unordered_map<std::string, Mix_Music*> m_Musics;
};

} // namespace MoLin
