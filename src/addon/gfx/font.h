#pragma once

#include "frontend/gfx/font.h"

#include <napi.h>

namespace Addon::Gfx {

class Font : public Napi::ObjectWrap<Font> {
public:
    static Napi::FunctionReference constructor;
    ::Gfx::Font m_font;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    Font(const Napi::CallbackInfo& info);
    ~Font();

    Napi::Value LoadFromFile(const Napi::CallbackInfo& info);
    Napi::Value LoadFromMemory(const Napi::CallbackInfo& info);
    Napi::Value IsValid(const Napi::CallbackInfo& info);
    Napi::Value CacheGlyphRange(const Napi::CallbackInfo& info);
    Napi::Value RenderText(const Napi::CallbackInfo& info);

private:
    void DestroyTextures();
};

}
