#include "addon/mixer/music.h"
#include "addon/param.h"
#include "addon/safe_call.h"

using namespace Param;

namespace Addon::Mixer {

Napi::FunctionReference Music::constructor;

Napi::Object Music::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "Music", {
        InstanceMethod("openFromFile", &Music::OpenFromFile),
        InstanceMethod("openFromMemory", &Music::OpenFromMemory),
        InstanceMethod("play", &Music::Play),
        InstanceMethod("fadeIn", &Music::FadeIn),
        StaticMethod("getVolume", &Music::GetVolume),
        StaticMethod("setVolume", &Music::SetVolume),
        StaticMethod("stop", &Music::Stop),
        StaticMethod("fadeOut", &Music::FadeOut),
        StaticMethod("pause", &Music::Pause),
        StaticMethod("resume", &Music::Resume),
        StaticMethod("rewind", &Music::Rewind),
        StaticMethod("isPaused", &Music::IsPaused),
        StaticMethod("setPosition", &Music::SetPosition),
        StaticMethod("isPlaying", &Music::IsPlaying)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Music", func);

    return exports;
}

Music::Music(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Music>{info}
{}

Napi::Value Music::OpenFromFile(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_music = {AsPath(info[0])};

        return info.Env().Undefined();
    });
}

Napi::Value Music::OpenFromMemory(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        auto buf = info[0].As<Napi::Uint8Array>();
        m_buf_ref = Napi::Persistent(buf);
        m_music = {buf.Data(), buf.ByteLength()};

        return info.Env().Undefined();
    });
}

Napi::Value Music::Play(const Napi::CallbackInfo& info)
{
    m_music.Play(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Music::FadeIn(const Napi::CallbackInfo& info)
{
    m_music.FadeIn(AsS32(info[0]), AsS32(info[1]));

    return info.Env().Undefined();
}

Napi::Value Music::GetVolume(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), SDL::Music::GetVolume());
}

Napi::Value Music::SetVolume(const Napi::CallbackInfo& info)
{
    SDL::Music::SetVolume(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Music::Stop(const Napi::CallbackInfo& info)
{
    SDL::Music::Stop();

    return info.Env().Undefined();
}

Napi::Value Music::FadeOut(const Napi::CallbackInfo& info)
{
    SDL::Music::FadeOut(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Music::Pause(const Napi::CallbackInfo& info)
{
    SDL::Music::Pause();

    return info.Env().Undefined();
}

Napi::Value Music::Resume(const Napi::CallbackInfo& info)
{
    SDL::Music::Resume();

    return info.Env().Undefined();
}

Napi::Value Music::Rewind(const Napi::CallbackInfo& info)
{
    SDL::Music::Rewind();

    return info.Env().Undefined();
}

Napi::Value Music::IsPaused(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), SDL::Music::IsPaused());
}

Napi::Value Music::SetPosition(const Napi::CallbackInfo& info)
{
    SDL::Music::SetPosition(AsF64(info[0]));

    return info.Env().Undefined();
}

Napi::Value Music::IsPlaying(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), SDL::Music::IsPlaying());
}

}
