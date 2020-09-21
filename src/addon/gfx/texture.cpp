#include "addon/gfx/texture.h"
#include "addon/param.h"
#include "addon/safe_call.h"
#include "frontend/app.h"

#include <fmt/format.h>

using namespace Param;

namespace Addon::Gfx {

Napi::FunctionReference Texture::constructor;

Napi::Object Texture::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "Texture", {
        InstanceAccessor<&Texture::GetId>("id"),
        InstanceAccessor<&Texture::GetWidth>("width"),
        InstanceAccessor<&Texture::GetHeight>("height"),
        InstanceMethod("loadFromFile", &Texture::LoadFromFile),
        InstanceMethod("loadFromMemoryRGBA32", &Texture::LoadFromMemoryRGBA32),
        InstanceMethod("isValid", &Texture::IsValid)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Texture", func);

    return exports;
}

Napi::Object Texture::Create(Napi::Env env, const ::Gfx::Texture& texture, bool owner)
{
    return constructor.New({
        FromU32(env, texture.GetId()),
		FromS32(env, texture.GetWidth()),
		FromS32(env, texture.GetHeight()),
        FromBool(env, owner)
    });
}

Texture::Texture(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Texture>{info}
{
    if (info.Length() > 0) {
        m_texture = {AsU32(info[0]), AsS32(info[1]), AsS32(info[2])};
        m_owner = AsBool(info[3]);
    }
}

Texture::~Texture()
{
    if (m_texture.GetId() == 0 || !m_owner)
        return;

    Frontend::App::GetInstance().DestroyTextureLater(m_texture.GetId());
}

Napi::Value Texture::GetId(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), m_texture.GetId());
}

Napi::Value Texture::GetWidth(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), m_texture.GetWidth());
}

Napi::Value Texture::GetHeight(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), m_texture.GetHeight());
}

Napi::Value Texture::LoadFromFile(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]()
    {
        try {
            m_texture.LoadFromFile(AsPath(info[0]));
        }
        catch (const std::exception& e) {
            throw std::runtime_error{fmt::format("Failed to load texture from file. {}", e.what())};
        }

        m_texture.SetFilter(static_cast<::Gfx::TextureFilter>(AsS32Or(info[1], static_cast<s32>(::Gfx::TextureFilter::Nearest))));

        return info.Env().Undefined();
    });
}

Napi::Value Texture::LoadFromMemoryRGBA32(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]()
    {
        m_texture.LoadFromMemoryRGBA32(AsS32(info[0]), AsS32(info[1]), info[2].As<Napi::Uint8Array>().Data());
        m_texture.SetFilter(static_cast<::Gfx::TextureFilter>(AsS32Or(info[3], static_cast<s32>(::Gfx::TextureFilter::Nearest))));

        return info.Env().Undefined();
    });
}

Napi::Value Texture::IsValid(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), m_texture.IsValid());
}

}
