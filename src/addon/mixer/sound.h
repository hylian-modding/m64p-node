#pragma once

#include "sdl/sound.h"

#include <napi.h>

namespace Addon::Mixer {

struct Sound : Napi::ObjectWrap<Sound> {
    static Napi::FunctionReference constructor;
    SDL::Sound m_sound;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    Sound(const Napi::CallbackInfo& info);

    Napi::Value LoadFromFile(const Napi::CallbackInfo& info);
    Napi::Value LoadFromMemory(const Napi::CallbackInfo& info);
    Napi::Value Play(const Napi::CallbackInfo& info);
    Napi::Value FadeIn(const Napi::CallbackInfo& info);
    Napi::Value GetVolume(const Napi::CallbackInfo& info);
    Napi::Value SetVolume(const Napi::CallbackInfo& info);
    static Napi::Value SetVolumeAll(const Napi::CallbackInfo& info);
    Napi::Value Stop(const Napi::CallbackInfo& info);
    static Napi::Value StopAll(const Napi::CallbackInfo& info);
    Napi::Value Expire(const Napi::CallbackInfo& info);
    static Napi::Value ExpireAll(const Napi::CallbackInfo& info);
    Napi::Value FadeOut(const Napi::CallbackInfo& info);
    static Napi::Value FadeOutAll(const Napi::CallbackInfo& info);
    Napi::Value Pause(const Napi::CallbackInfo& info);
    static Napi::Value PauseAll(const Napi::CallbackInfo& info);
    Napi::Value Resume(const Napi::CallbackInfo& info);
    static Napi::Value ResumeAll(const Napi::CallbackInfo& info);
    Napi::Value IsPaused(const Napi::CallbackInfo& info);
    static Napi::Value IsAnyPaused(const Napi::CallbackInfo& info);
    Napi::Value IsPlaying(const Napi::CallbackInfo& info);
    static Napi::Value IsAnyPlaying(const Napi::CallbackInfo& info);
    Napi::Value SetPanning(const Napi::CallbackInfo& info);
    Napi::Value SetPosition(const Napi::CallbackInfo& info);
    Napi::Value SetDistance(const Napi::CallbackInfo& info);
};

}
