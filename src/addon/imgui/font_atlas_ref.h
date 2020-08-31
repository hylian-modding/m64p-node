#pragma once

#include "imgui/imgui.h"

#include <napi.h>

namespace Addon::ImGui_ {

struct FontAtlasRef : Napi::ObjectWrap<FontAtlasRef> {
    static Napi::FunctionReference constructor;
	ImFontAtlas* m_font_atlas;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
	static Napi::Object Create(Napi::Env env, ImFontAtlas* font_atlas);

    FontAtlasRef(const Napi::CallbackInfo& info);

	Napi::Value AddFontFromFile(const Napi::CallbackInfo& info);
	Napi::Value AddFontFromMemory(const Napi::CallbackInfo& info);
	Napi::Value AddFontFromMemoryCompressed(const Napi::CallbackInfo& info);
};

}
