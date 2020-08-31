#include "addon/mixer/sound.h"
#include "addon/param.h"
#include "addon/safe_call.h"

using namespace Param;

namespace Addon::Mixer {

Napi::FunctionReference Sound::constructor;

Napi::Object Sound::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "Sound", {
        InstanceMethod("loadFromFile", &Sound::LoadFromFile),
        InstanceMethod("loadFromMemory", &Sound::LoadFromMemory),
        InstanceMethod("play", &Sound::Play),
        InstanceMethod("fadeIn", &Sound::FadeIn),
        InstanceMethod("getVolume", &Sound::GetVolume),
        InstanceMethod("setVolume", &Sound::SetVolume),
        StaticMethod("setVolume", &Sound::SetVolumeAll),
        InstanceMethod("stop", &Sound::Stop),
        StaticMethod("stop", &Sound::StopAll),
        InstanceMethod("expire", &Sound::Expire),
        StaticMethod("expire", &Sound::ExpireAll),
        InstanceMethod("fadeOut", &Sound::FadeOut),
        StaticMethod("fadeOut", &Sound::FadeOutAll),
        InstanceMethod("pause", &Sound::Pause),
        StaticMethod("pause", &Sound::PauseAll),
        InstanceMethod("resume", &Sound::Resume),
        StaticMethod("resume", &Sound::ResumeAll),
        InstanceMethod("isPaused", &Sound::IsPaused),
        StaticMethod("isPaused", &Sound::IsAnyPaused),
        InstanceMethod("isPlaying", &Sound::IsPlaying),
        StaticMethod("isPlaying", &Sound::IsAnyPlaying),
        InstanceMethod("setPanning", &Sound::SetPanning),
        InstanceMethod("setPosition", &Sound::SetPosition),
        InstanceMethod("setDistance", &Sound::SetDistance)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Sound", func);

    return exports;
}

Sound::Sound(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Sound>{info}
{}

Napi::Value Sound::LoadFromFile(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_sound = {AsPath(info[0])};

        return info.Env().Undefined();
    });
}

Napi::Value Sound::LoadFromMemory(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        auto buf = info[0].As<Napi::Uint8Array>();
        m_sound = {buf.Data(), buf.ByteLength()};

        return info.Env().Undefined();
    });
}

Napi::Value Sound::Play(const Napi::CallbackInfo& info)
{
    m_sound.Play(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Sound::FadeIn(const Napi::CallbackInfo& info)
{
    m_sound.FadeIn(AsS32(info[0]), AsS32(info[1]));

    return info.Env().Undefined();
}

Napi::Value Sound::GetVolume(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), m_sound.GetVolume());
}

Napi::Value Sound::SetVolume(const Napi::CallbackInfo& info)
{
    m_sound.SetVolume(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Sound::SetVolumeAll(const Napi::CallbackInfo& info)
{
    SDL::Sound::SetVolumeAll(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Sound::Stop(const Napi::CallbackInfo& info)
{
    m_sound.Stop();

    return info.Env().Undefined();
}

Napi::Value Sound::StopAll(const Napi::CallbackInfo& info)
{
    SDL::Sound::StopAll();

    return info.Env().Undefined();
}

Napi::Value Sound::Expire(const Napi::CallbackInfo& info)
{
    m_sound.Expire(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Sound::ExpireAll(const Napi::CallbackInfo& info)
{
    SDL::Sound::ExpireAll(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Sound::FadeOut(const Napi::CallbackInfo& info)
{
    m_sound.FadeOut(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Sound::FadeOutAll(const Napi::CallbackInfo& info)
{
    SDL::Sound::FadeOutAll(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value Sound::Pause(const Napi::CallbackInfo& info)
{
    m_sound.Pause();

    return info.Env().Undefined();
}

Napi::Value Sound::PauseAll(const Napi::CallbackInfo& info)
{
    SDL::Sound::PauseAll();

    return info.Env().Undefined();
}

Napi::Value Sound::Resume(const Napi::CallbackInfo& info)
{
    m_sound.Resume();

    return info.Env().Undefined();
}

Napi::Value Sound::ResumeAll(const Napi::CallbackInfo& info)
{
    SDL::Sound::ResumeAll();

    return info.Env().Undefined();
}

Napi::Value Sound::IsPaused(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), m_sound.IsPaused());
}

Napi::Value Sound::IsAnyPaused(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), SDL::Sound::IsAnyPaused());
}

Napi::Value Sound::IsPlaying(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), m_sound.IsPlaying());
}

Napi::Value Sound::IsAnyPlaying(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), SDL::Sound::IsAnyPlaying());
}

Napi::Value Sound::SetPanning(const Napi::CallbackInfo& info)
{
    m_sound.SetPanning(AsF32(info[0]), AsF32(info[1]));

    return info.Env().Undefined();
}

Napi::Value Sound::SetPosition(const Napi::CallbackInfo& info)
{
    m_sound.SetPosition(AsS32(info[0]), AsF32(info[1]));

    return info.Env().Undefined();
}

Napi::Value Sound::SetDistance(const Napi::CallbackInfo& info)
{
    m_sound.SetDistance(AsF32(info[0]));

    return info.Env().Undefined();
}

}
