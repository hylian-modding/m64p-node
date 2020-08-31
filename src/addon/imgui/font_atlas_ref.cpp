#include "addon/imgui/font_atlas_ref.h"
#include "addon/imgui/font_ref.h"
#include "addon/param.h"
#include "frontend/app.h"

using namespace Param;

namespace Addon::ImGui_ {

inline Frontend::App& GetApp()
{
    return Frontend::App::GetInstance();
}

Napi::FunctionReference FontAtlasRef::constructor;

Napi::Object FontAtlasRef::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "FontAtlasRef", {
		InstanceMethod("addFontFromFile", &FontAtlasRef::AddFontFromFile),
		InstanceMethod("addFontFromMemory", &FontAtlasRef::AddFontFromMemory),
		InstanceMethod("addFontFromMemoryCompressed", &FontAtlasRef::AddFontFromMemoryCompressed)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("FontAtlasRef", func);

    return exports;
}

Napi::Object FontAtlasRef::Create(Napi::Env env, ImFontAtlas* font_atlas)
{
    return constructor.New({FromPtr(env, font_atlas)});
}

FontAtlasRef::FontAtlasRef(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<FontAtlasRef>{info}
{
	m_font_atlas = static_cast<ImFontAtlas*>(AsPtr(info[0]));
}

Napi::Value FontAtlasRef::AddFontFromFile(const Napi::CallbackInfo& info)
{
    auto fnt = m_font_atlas->AddFontFromFileTTF(
        AsPath(info[0]).string().c_str(), AsF32(info[1]));
    GetApp().WantRebuildFontAtlas();

	return FontRef::Create(info.Env(), fnt);
}

Napi::Value FontAtlasRef::AddFontFromMemory(const Napi::CallbackInfo& info)
{
	auto font = info[0].As<Napi::Uint8Array>();
    auto fnt = m_font_atlas->AddFontFromMemoryTTF(
        font.Data(), static_cast<int>(font.ByteLength()), AsF32(info[1]));
    GetApp().WantRebuildFontAtlas();

	return FontRef::Create(info.Env(), fnt);
}

Napi::Value FontAtlasRef::AddFontFromMemoryCompressed(const Napi::CallbackInfo& info)
{
	auto font = info[0].As<Napi::Uint8Array>();
    auto fnt = m_font_atlas->AddFontFromMemoryCompressedTTF(
        font.Data(), static_cast<int>(font.ByteLength()), AsF32(info[1]));
    GetApp().WantRebuildFontAtlas();

	return FontRef::Create(info.Env(), fnt);
}

}
