#pragma once

#include "common/types.h"

#include <filesystem>
#include <memory>

typedef struct _Mix_Music Mix_Music;

namespace SDL {

class Music {
public:
    Music();
    Music(const std::filesystem::path& path);
    Music(const u8* data, std::size_t size);

	Mix_Music* Get() const;
    void Play(int loops);
    void FadeIn(int loops, int ms);
    static float GetVolume();
    static void SetVolume(float volume);
    static void Stop();
    static void FadeOut(int ms);
    static void Pause();
    static void Resume();
    static void Rewind();
    static bool IsPaused();
    static void SetPosition(double position);
    static bool IsPlaying();

private:
    std::unique_ptr<Mix_Music, void(*)(Mix_Music*)> m_ptr;
};

}
