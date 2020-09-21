#include "addon/gfx/font.h"
#include "addon/gfx/texture.h"
#include "addon/imgui/param.h"
#include "addon/param.h"
#include "addon/safe_call.h"
#include "frontend/app.h"

#include <fmt/format.h>

using namespace Param;
using namespace Addon::ImGui_::Param;

namespace Addon::Gfx {

Napi::FunctionReference Font::constructor;

Napi::Object Font::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "Font", {
        InstanceMethod("loadFromFile", &Font::LoadFromFile),
        InstanceMethod("loadFromMemory", &Font::LoadFromMemory),
        InstanceMethod("cacheGlyphRange", &Font::CacheGlyphRange),
        InstanceMethod("renderText", &Font::RenderText),
        InstanceMethod("isValid", &Font::IsValid),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Font", func);

    return exports;
}

Font::Font(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Font>{info}
{}

Font::~Font()
{
    DestroyTextures();
}

void Font::DestroyTextures()
{
    auto v = m_font.CollectTextures();
    auto& a = Frontend::App::GetInstance();

    for (auto id : v)
        a.DestroyTextureLater(id);
}

Napi::Value Font::LoadFromFile(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        DestroyTextures();

        try {
            m_font.LoadFromFile(AsPath(info[0]), AsS32(info[1]), AsS32(info[2]));
        }
        catch (const std::exception& e) {
            throw std::runtime_error{fmt::format("Failed to load font from file. {}", e.what())};
        }

        return info.Env().Undefined();
    });
}

Napi::Value Font::LoadFromMemory(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        DestroyTextures();
        auto buf = info[0].As<Napi::Uint8Array>();
        m_font.LoadFromMemory(buf.Data(), buf.ByteLength(), AsS32(info[1]), AsS32(info[2]));

        return info.Env().Undefined();
    });
}

Napi::Value Font::IsValid(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), m_font.IsValid());
}

Napi::Value Font::CacheGlyphRange(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_font.CacheGlyphRange(AsU16(info[0]), AsU16(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value Font::RenderText(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        return Texture::Create(info.Env(), m_font.RenderText(AsStrUtf8(info[0]), AsColor(info[1]), AsColor(info[2]),
            static_cast<::Gfx::TextureFilter>(AsS32Or(info[3], static_cast<s32>(::Gfx::TextureFilter::Nearest)))), true);
    });
}

}
