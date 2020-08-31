#pragma once

#include "imgui/imgui.h"

#include <napi.h>

namespace Addon::ImGui_ {

struct ViewportRef : Napi::ObjectWrap<ViewportRef> {
    static Napi::FunctionReference constructor;
	ImGuiViewport* m_viewport;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object Create(Napi::Env env, ImGuiViewport* viewport);

    ViewportRef(const Napi::CallbackInfo& info);

	Napi::Value GetId(const Napi::CallbackInfo& info);
	Napi::Value GetFlags(const Napi::CallbackInfo& info);
	void SetFlags(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetPos(const Napi::CallbackInfo& info);
	void SetPos(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetSize(const Napi::CallbackInfo& info);
	void SetSize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetWorkOffsetMin(const Napi::CallbackInfo& info);
	void SetWorkOffsetMin(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetWorkOffsetMax(const Napi::CallbackInfo& info);
	void SetWorkOffsetMax(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetDPIScale(const Napi::CallbackInfo& info);
	void SetDPIScale(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetParentViewportId(const Napi::CallbackInfo& info);
	Napi::Value GetPlatformHandle(const Napi::CallbackInfo& info);
	Napi::Value GetCenter(const Napi::CallbackInfo& info);
	Napi::Value GetWorkPos(const Napi::CallbackInfo& info);
	Napi::Value GetWorkSize(const Napi::CallbackInfo& info);
};

}
