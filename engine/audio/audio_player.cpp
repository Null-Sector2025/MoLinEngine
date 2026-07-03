#include "audio_player.h"
#include <iostream>

namespace MoLin {

void AudioPlayer::PlaySound(Mix_Chunk* chunk, int loops) { if (chunk) Mix_PlayChannel(-1, chunk, loops); }
void AudioPlayer::PlayMusic(Mix_Music* music, int loops) { if (music) Mix_PlayMusic(music, loops); }
void AudioPlayer::StopMusic() { Mix_HaltMusic(); }
void AudioPlayer::PauseMusic() { Mix_PauseMusic(); }
void AudioPlayer::ResumeMusic() { Mix_ResumeMusic(); }
void AudioPlayer::SetVolume(int volume) { Mix_VolumeMusic(volume); }

} // namespace MoLin
