#pragma once

#include "sdl/music.h"

#include <napi.h>

namespace Addon::Mixer {

struct Music : Napi::ObjectWrap<Music> {
    static Napi::FunctionReference constructor;
    SDL::Music m_music;
    Napi::Reference<Napi::Uint8Array> m_buf_ref;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    Music(const Napi::CallbackInfo& info);

    Napi::Value OpenFromFile(const Napi::CallbackInfo& info);
    Napi::Value OpenFromMemory(const Napi::CallbackInfo& info);
    Napi::Value Play(const Napi::CallbackInfo& info);
    Napi::Value FadeIn(const Napi::CallbackInfo& info);
    static Napi::Value GetVolume(const Napi::CallbackInfo& info);
    static Napi::Value SetVolume(const Napi::CallbackInfo& info);
    static Napi::Value Stop(const Napi::CallbackInfo& info);
    static Napi::Value FadeOut(const Napi::CallbackInfo& info);
    static Napi::Value Pause(const Napi::CallbackInfo& info);
    static Napi::Value Resume(const Napi::CallbackInfo& info);
    static Napi::Value Rewind(const Napi::CallbackInfo& info);
    static Napi::Value IsPaused(const Napi::CallbackInfo& info);
    static Napi::Value SetPosition(const Napi::CallbackInfo& info);
    static Napi::Value IsPlaying(const Napi::CallbackInfo& info);
};

}
