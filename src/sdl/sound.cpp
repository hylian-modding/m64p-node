#include "sdl/error.h"
#include "sdl/sound.h"

#include <algorithm>

#include <SDL2/SDL_mixer.h>

namespace SDL {

Sound::Sound() :
	m_ptr{nullptr, Mix_FreeChunk}
{}

Sound::Sound(const std::filesystem::path& path) :
    Sound{}
{
    m_ptr.reset(Mix_LoadWAV(path.string().c_str()));

    if (!m_ptr)
        throw Error{"Mix_LoadWAV"};
}

Sound::Sound(const u8* data, std::size_t size) :
    Sound{}
{
    m_ptr.reset(Mix_LoadWAV_RW(SDL_RWFromConstMem(data, static_cast<int>(size)), true));

    if (!m_ptr)
        throw Error{"Mix_LoadWAV_RW"};
}

Mix_Chunk* Sound::Get() const
{
    return m_ptr.get();
}

void Sound::Play(int loops)
{
    m_channel = Mix_PlayChannel(-1, Get(), loops);
}

void Sound::FadeIn(int loops, int ms)
{
    m_channel = Mix_FadeInChannel(-1, Get(), loops, ms);
}

float Sound::GetVolume()
{
    return m_channel >= 0 ? Mix_Volume(m_channel, -1) / static_cast<float>(MIX_MAX_VOLUME) : 0.0f;
}

void Sound::SetVolume(float volume)
{
    if (m_channel >= 0)
        Mix_Volume(m_channel, static_cast<int>(std::clamp(volume * static_cast<float>(MIX_MAX_VOLUME), 0.0f, static_cast<float>(MIX_MAX_VOLUME))));
}

void Sound::SetVolumeAll(float volume)
{
    Mix_Volume(-1, static_cast<int>(std::clamp(volume * static_cast<float>(MIX_MAX_VOLUME), 0.0f, static_cast<float>(MIX_MAX_VOLUME))));
}

void Sound::Stop()
{
    if (m_channel >= 0)
        Mix_HaltChannel(m_channel);
}

void Sound::StopAll()
{
    Mix_HaltChannel(-1);
}

void Sound::Expire(int ms)
{
    if (m_channel >= 0)
        Mix_ExpireChannel(m_channel, ms);
}

void Sound::ExpireAll(int ms)
{
    Mix_ExpireChannel(-1, ms);
}

void Sound::FadeOut(int ms)
{
    if (m_channel >= 0)
        Mix_FadeOutChannel(m_channel, ms);
}

void Sound::FadeOutAll(int ms)
{
    Mix_FadeOutChannel(-1, ms);
}

void Sound::Pause()
{
    if (m_channel >= 0)
        Mix_Pause(m_channel);
}

void Sound::PauseAll()
{
    Mix_Pause(-1);
}

void Sound::Resume()
{
    if (m_channel >= 0)
        Mix_Resume(m_channel);
}

void Sound::ResumeAll()
{
    Mix_Resume(-1);
}

bool Sound::IsPaused()
{
    return m_channel >= 0 ? Mix_Paused(m_channel) : true;
}

bool Sound::IsAnyPaused()
{
    return Mix_Paused(-1) > 0;
}

bool Sound::IsPlaying()
{
    return m_channel >= 0 ? Mix_Playing(m_channel) : false;
}

bool Sound::IsAnyPlaying()
{
    return Mix_Playing(-1) > 0;
}

void Sound::SetPanning(float left, float right)
{
    if (m_channel >= 0)
        Mix_SetPanning(m_channel, static_cast<Uint8>(std::clamp(left * 255.0f, 0.0f, 255.0f)), static_cast<Uint8>(std::clamp(right * 255.0f, 0.0f, 255.0f)));
}

void Sound::SetPosition(int angle, float distance)
{
    if (m_channel >= 0)
        Mix_SetPosition(m_channel, static_cast<Sint16>(angle), static_cast<Uint8>(std::clamp(distance * 255.0f, 0.0f, 255.0f)));
}

void Sound::SetDistance(float distance)
{
    if (m_channel >= 0)
        Mix_SetDistance(m_channel, static_cast<Uint8>(std::clamp(distance * 255.0f, 0.0f, 255.0f)));
}

}
