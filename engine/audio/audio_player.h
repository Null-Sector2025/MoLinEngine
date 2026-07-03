#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

namespace MoLin {

class AudioPlayer {
public:
    static AudioPlayer& Instance() {
        static AudioPlayer instance;
        return instance;
    }

    void PlaySound(Mix_Chunk* chunk, int loops = 0);
    void PlayMusic(Mix_Music* music, int loops = -1);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    void SetVolume(int volume); // 0-128

private:
    AudioPlayer() = default;
};

} // namespace MoLin
