#include "addon/gfx/font.h"
#include "addon/gfx/texture.h"
#include "addon/imgui/draw_list_ref.h"
#include "addon/imgui/param.h"
#include "addon/param.h"
#include "frontend/gfx/gfx.h"

using namespace Param;
using namespace Addon::ImGui_::Param;

namespace Addon::Gfx {

Napi::Value AddSprite(const Napi::CallbackInfo& info)
{
    auto dl = ImGui_::DrawListRef::Unwrap(AsObject(info[0]));
    auto& tex = Texture::Unwrap(AsObject(info[1]))->m_texture;

    ::Gfx::AddSprite(dl->m_draw_list, dl->m_origin, tex, AsVec4(info[2]), AsVec4(info[3]), AsColor(info[4]),
        static_cast<::Gfx::FlipFlags>(AsS32(info[5])));

    return info.Env().Undefined();
}

Napi::Value AddSpriteRotated(const Napi::CallbackInfo& info)
{
    auto dl = ImGui_::DrawListRef::Unwrap(AsObject(info[0]));
    auto& tex = Texture::Unwrap(AsObject(info[1]))->m_texture;

    ::Gfx::AddSpriteRotated(dl->m_draw_list, dl->m_origin, tex, AsVec4(info[2]), AsVec4(info[3]), AsColor(info[4]),
        static_cast<::Gfx::FlipFlags>(AsS32(info[5])), AsF32(info[6]));

    return info.Env().Undefined();
}

Napi::Value CalcSpriteRotatedBounds(const Napi::CallbackInfo& info)
{
    return FromVec4(info.Env(), ::Gfx::CalcSpriteRotatedBounds(AsVec4(info[0]), AsF32(info[1])));
}

Napi::Value AddText(const Napi::CallbackInfo& info)
{
    auto dl = ImGui_::DrawListRef::Unwrap(AsObject(info[0]));
    auto& font = Font::Unwrap(AsObject(info[1]))->m_font;

    LoadStrUtf8(0, info[2]);
    char* str = GetStrUtf8(0);

    ::Gfx::AddText(dl->m_draw_list, dl->m_origin, font, str, str + std::strlen(str), AsVec2(info[3]),
        AsColor(info[4]), AsColor(info[5]), AsVec2(info[6]));

    return info.Env().Undefined();
}

Napi::Value CalcTextSize(const Napi::CallbackInfo& info)
{
    auto& font = Font::Unwrap(AsObject(info[0]))->m_font;

    LoadStrUtf8(0, info[1]);
    char* str = GetStrUtf8(0);

    return FromVec2(info.Env(), ::Gfx::CalcTextSize(font, str, str + std::strlen(str), AsVec2(info[2])));
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    Font::Init(env, exports);
    Texture::Init(env, exports);

    exports.Set("addSprite", Napi::Function::New(env, AddSprite));
    exports.Set("addSpriteRotated", Napi::Function::New(env, AddSpriteRotated));
    exports.Set("calcSpriteRotatedBounds", Napi::Function::New(env, CalcSpriteRotatedBounds));
    exports.Set("addText", Napi::Function::New(env, AddText));
    exports.Set("calcTextSize", Napi::Function::New(env, CalcTextSize));

    return exports;
}

}
