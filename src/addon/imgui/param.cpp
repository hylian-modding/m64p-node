#include "addon/imgui/draw_list_ref.h"
#include "addon/imgui/font_ref.h"
#include "addon/imgui/param.h"
#include "addon/imgui/viewport_ref.h"
#include "addon/param.h"

using namespace Param;

namespace Addon::ImGui_::Param {

ImVec2 AsVec2(const Napi::Value& value)
{
    auto obj = AsObject(value);
    return {AsF32(obj.Get("x")), AsF32(obj.Get("y"))};
}

ImVec2 AsVec2Or(const Napi::Value& value, const ImVec2& default_value)
{
    return value.IsUndefined() ? default_value : AsVec2(value);
}

std::vector<ImVec2> AsVec2Array(const Napi::Value& value)
{
    auto a = AsArray(value);
	std::vector<ImVec2> v(a.Length());

	for (std::size_t i{}; i < v.size(); ++i)
		v[i] = AsVec2(a.Get(static_cast<u32>(i)));

	return v;
}

Napi::Value FromVec2(Napi::Env env, const ImVec2& value)
{
    auto obj = Napi::Object::New(env);
    obj.Set("x", FromF32(env, value.x));
    obj.Set("y", FromF32(env, value.y));

    return obj;
}

Napi::Value FromVec2(Napi::Env env, int x, int y)
{
    auto obj = Napi::Object::New(env);
    obj.Set("x", FromS32(env, x));
    obj.Set("y", FromS32(env, y));

    return obj;
}

ImVec4 AsVec4(const Napi::Value& value)
{
    auto obj = AsObject(value);
    return {AsF32(obj.Get("x")), AsF32(obj.Get("y")), AsF32(obj.Get("z")), AsF32(obj.Get("w"))};
}

Napi::Value FromVec4(Napi::Env env, const ImVec4& value)
{
    auto obj = Napi::Object::New(env);
    obj.Set("x", FromF32(env, value.x));
    obj.Set("y", FromF32(env, value.y));
    obj.Set("z", FromF32(env, value.z));
    obj.Set("w", FromF32(env, value.w));

    return obj;
}

Napi::Value FromVec4(Napi::Env env, int x, int y, int z, int w)
{
    auto obj = Napi::Object::New(env);
    obj.Set("x", FromS32(env, x));
    obj.Set("y", FromS32(env, y));
    obj.Set("z", FromS32(env, z));
    obj.Set("w", FromS32(env, w));

    return obj;
}

ImColor AsColor(const Napi::Value& value)
{
    auto obj = AsObject(value);
    return {AsF32(obj.Get("x")), AsF32(obj.Get("y")), AsF32(obj.Get("z")), AsF32(obj.Get("w"))};
}

ImColor AsColorOr(const Napi::Value& value, const ImColor& default_value)
{
    return value.IsUndefined() ? default_value : AsColor(value);
}

Napi::Value FromColor(Napi::Env env, const ImColor& value)
{
    auto obj = Napi::Object::New(env);
    obj.Set("x", FromF32(env, value.Value.x));
    obj.Set("y", FromF32(env, value.Value.y));
    obj.Set("z", FromF32(env, value.Value.z));
    obj.Set("w", FromF32(env, value.Value.w));

    return obj;
}

void FromColor(Napi::Env env, Napi::Object& obj, const ImColor& value)
{
    obj.Set("x", FromF32(env, value.Value.x));
    obj.Set("y", FromF32(env, value.Value.y));
    obj.Set("z", FromF32(env, value.Value.z));
    obj.Set("w", FromF32(env, value.Value.w));
}

u32 AsU32Color(const Napi::Value& value)
{
    return ImGui::GetColorU32(AsColor(value).Value);
}

u32 AsU32ColorOr(const Napi::Value& value, u32 default_value)
{
    return value.IsUndefined() ? default_value : AsU32Color(value);
}

ImFont* AsFontPtr(const Napi::Value& value)
{
    return FontRef::Unwrap(AsObject(value))->m_font;
}

ImFont* AsFontPtrOr(const Napi::Value& value, ImFont* default_value)
{
    return value.IsUndefined() ? default_value : AsFontPtr(value);
}

ImGuiViewport* AsViewportPtr(const Napi::Value& value)
{
    return ViewportRef::Unwrap(AsObject(value))->m_viewport;
}

ImGuiViewport* AsViewportPtrOr(const Napi::Value& value, ImGuiViewport* default_value)
{
    return value.IsUndefined() ? default_value : AsViewportPtr(value);
}

ImDrawList* AsDrawListPtr(const Napi::Value& value)
{
    return DrawListRef::Unwrap(AsObject(value))->m_draw_list;
}

}
