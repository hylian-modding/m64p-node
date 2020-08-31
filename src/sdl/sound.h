#pragma once

#include "common/types.h"

#include <filesystem>
#include <memory>

struct Mix_Chunk;

namespace SDL {

class Sound {
public:
    Sound();
    Sound(const std::filesystem::path& path);
    Sound(const u8* data, std::size_t size);

	Mix_Chunk* Get() const;
    void Play(int loops);
    void FadeIn(int loops, int ms);
    float GetVolume();
    void SetVolume(float volume);
    static void SetVolumeAll(float volume);
    void Stop();
    static void StopAll();
    void Expire(int ms);
    static void ExpireAll(int ms);
    void FadeOut(int ms);
    static void FadeOutAll(int ms);
    void Pause();
    static void PauseAll();
    void Resume();
    static void ResumeAll();
    bool IsPaused();
    static bool IsAnyPaused();
    bool IsPlaying();
    static bool IsAnyPlaying();
    void SetPanning(float left, float right);
    void SetPosition(int angle, float distance);
    void SetDistance(float distance);

private:
    std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk*)> m_ptr;
    int m_channel{-1};
};

}
