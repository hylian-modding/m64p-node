#include "addon/imgui/font_ref.h"
#include "addon/param.h"

using namespace Param;

namespace Addon::ImGui_ {

Napi::FunctionReference FontRef::constructor;

Napi::Object FontRef::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "FontRef", {
		InstanceAccessor<&FontRef::GetSize>("size"),
		InstanceAccessor<&FontRef::GetScale, &FontRef::SetScale>("scale")
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("FontRef", func);

    return exports;
}

Napi::Object FontRef::Create(Napi::Env env, ImFont* font)
{
    return constructor.New({FromPtr(env, font)});
}

FontRef::FontRef(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<FontRef>{info}
{
	m_font = static_cast<ImFont*>(AsPtr(info[0]));
}

Napi::Value FontRef::GetSize(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_font->FontSize);
}

Napi::Value FontRef::GetScale(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_font->Scale);
}

void FontRef::SetScale(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_font->Scale = AsF32(value);
}

}
