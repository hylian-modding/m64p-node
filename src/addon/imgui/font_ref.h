#pragma once

#include "imgui/imgui.h"

#include <napi.h>

namespace Addon::ImGui_ {

struct FontRef : Napi::ObjectWrap<FontRef> {
    static Napi::FunctionReference constructor;
	ImFont* m_font;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
	static Napi::Object Create(Napi::Env env, ImFont* font);

    FontRef(const Napi::CallbackInfo& info);

	Napi::Value GetSize(const Napi::CallbackInfo& info);
	Napi::Value GetScale(const Napi::CallbackInfo& info);
	void SetScale(const Napi::CallbackInfo& info, const Napi::Value& value);
};

}
