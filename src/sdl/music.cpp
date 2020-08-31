#include "sdl/error.h"
#include "sdl/music.h"

#include <algorithm>

#include <SDL2/SDL_mixer.h>

namespace SDL {

Music::Music() :
	m_ptr{nullptr, Mix_FreeMusic}
{}

Music::Music(const std::filesystem::path& path) :
    Music{}
{
    m_ptr.reset(Mix_LoadMUS(path.string().c_str()));

    if (!m_ptr)
        throw Error{"Mix_LoadMUS"};
}

Music::Music(const u8* data, std::size_t size) :
    Music{}
{
    m_ptr.reset(Mix_LoadMUS_RW(SDL_RWFromConstMem(data, static_cast<int>(size)), true));

    if (!m_ptr)
        throw Error{"Mix_LoadMUS_RW"};
}

Mix_Music* Music::Get() const
{
    return m_ptr.get();
}

void Music::Play(int loops)
{
    Mix_PlayMusic(Get(), loops);
}

void Music::FadeIn(int loops, int ms)
{
    Mix_FadeInMusic(Get(), loops, ms);
}

float Music::GetVolume()
{
    return Mix_VolumeMusic(-1) / static_cast<float>(MIX_MAX_VOLUME);
}

void Music::SetVolume(float volume)
{
    Mix_VolumeMusic(static_cast<int>(std::clamp(volume * static_cast<float>(MIX_MAX_VOLUME), 0.0f, static_cast<float>(MIX_MAX_VOLUME))));
}

void Music::Stop()
{
    Mix_HaltMusic();
}

void Music::FadeOut(int ms)
{
    Mix_FadeOutMusic(ms);
}

void Music::Pause()
{
    Mix_PauseMusic();
}

void Music::Resume()
{
    Mix_ResumeMusic();
}

void Music::Rewind()
{
    Mix_RewindMusic();
}

bool Music::IsPaused()
{
    return Mix_PausedMusic();
}

void Music::SetPosition(double position)
{
    Mix_SetMusicPosition(position);
}

bool Music::IsPlaying()
{
    return Mix_PlayingMusic();
}

}
