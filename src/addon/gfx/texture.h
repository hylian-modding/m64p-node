#pragma once

#include "frontend/gfx/texture.h"

#include <napi.h>

namespace Addon::Gfx {

struct Texture : Napi::ObjectWrap<Texture> {
    static Napi::FunctionReference constructor;
    ::Gfx::Texture m_texture;
    bool m_owner{};

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object Create(Napi::Env env, const ::Gfx::Texture& texture, bool owner);

    Texture(const Napi::CallbackInfo& info);
    ~Texture();

    Napi::Value GetId(const Napi::CallbackInfo& info);
    Napi::Value GetWidth(const Napi::CallbackInfo& info);
    Napi::Value GetHeight(const Napi::CallbackInfo& info);
    Napi::Value LoadFromFile(const Napi::CallbackInfo& info);
    Napi::Value LoadFromMemoryRGBA32(const Napi::CallbackInfo& info);
    Napi::Value IsValid(const Napi::CallbackInfo& info);
};

}
