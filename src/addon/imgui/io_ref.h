#pragma once

#include "imgui/imgui.h"

#include <napi.h>

namespace Addon::ImGui_ {

struct IORef : Napi::ObjectWrap<IORef> {
    static Napi::FunctionReference constructor;
	ImGuiIO* m_io;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
	static Napi::Object Create(Napi::Env env, ImGuiIO* io);

    IORef(const Napi::CallbackInfo& info);

	Napi::Value GetConfigFlags(const Napi::CallbackInfo& info);
	void SetConfigFlags(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetBackendFlags(const Napi::CallbackInfo& info);
	void SetBackendFlags(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetDisplaySize(const Napi::CallbackInfo& info);
	void SetDisplaySize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetDeltaTime(const Napi::CallbackInfo& info);
	void SetDeltaTime(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetIniSavingRate(const Napi::CallbackInfo& info);
	void SetIniSavingRate(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetIniFilename(const Napi::CallbackInfo& info);
	void SetIniFilename(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetLogFilename(const Napi::CallbackInfo& info);
	void SetLogFilename(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetMouseDoubleClickTime(const Napi::CallbackInfo& info);
	void SetMouseDoubleClickTime(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetMouseDoubleClickMaxDist(const Napi::CallbackInfo& info);
	void SetMouseDoubleClickMaxDist(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetMouseDragThreshold(const Napi::CallbackInfo& info);
	void SetMouseDragThreshold(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetKeyRepeatDelay(const Napi::CallbackInfo& info);
	void SetKeyRepeatDelay(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetKeyRepeatRate(const Napi::CallbackInfo& info);
	void SetKeyRepeatRate(const Napi::CallbackInfo& info, const Napi::Value& value);

	Napi::Value GetFonts(const Napi::CallbackInfo& info);
	Napi::Value GetFontGlobalScale(const Napi::CallbackInfo& info);
	void SetFontGlobalScale(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetFontAllowUserScaling(const Napi::CallbackInfo& info);
	void SetFontAllowUserScaling(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetFontDefault(const Napi::CallbackInfo& info);
	void SetFontDefault(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetDisplayFramebufferScale(const Napi::CallbackInfo& info);
	void SetDisplayFramebufferScale(const Napi::CallbackInfo& info, const Napi::Value& value);

	Napi::Value GetConfigDockingNoSplit(const Napi::CallbackInfo& info);
	void SetConfigDockingNoSplit(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigDockingWithShift(const Napi::CallbackInfo& info);
	void SetConfigDockingWithShift(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigDockingAlwaysTabBar(const Napi::CallbackInfo& info);
	void SetConfigDockingAlwaysTabBar(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigDockingTransparentPayload(const Napi::CallbackInfo& info);
	void SetConfigDockingTransparentPayload(const Napi::CallbackInfo& info, const Napi::Value& value);

	Napi::Value GetConfigViewportsNoAutoMerge(const Napi::CallbackInfo& info);
	void SetConfigViewportsNoAutoMerge(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigViewportsNoTaskBarIcon(const Napi::CallbackInfo& info);
	void SetConfigViewportsNoTaskBarIcon(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigViewportsNoDecoration(const Napi::CallbackInfo& info);
	void SetConfigViewportsNoDecoration(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigViewportsNoDefaultParent(const Napi::CallbackInfo& info);
	void SetConfigViewportsNoDefaultParent(const Napi::CallbackInfo& info, const Napi::Value& value);

	Napi::Value GetMouseDrawCursor(const Napi::CallbackInfo& info);
	void SetMouseDrawCursor(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigInputTextCursorBlink(const Napi::CallbackInfo& info);
	void SetConfigInputTextCursorBlink(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigWindowsResizeFromEdges(const Napi::CallbackInfo& info);
	void SetConfigWindowsResizeFromEdges(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigWindowsMoveFromTitleBarOnly(const Napi::CallbackInfo& info);
	void SetConfigWindowsMoveFromTitleBarOnly(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetConfigWindowsMemoryCompactTimer(const Napi::CallbackInfo& info);
	void SetConfigWindowsMemoryCompactTimer(const Napi::CallbackInfo& info, const Napi::Value& value);

	Napi::Value GetBackendPlatformName(const Napi::CallbackInfo& info);
	Napi::Value GetBackendRendererName(const Napi::CallbackInfo& info);

	Napi::Value GetWantCaptureMouse(const Napi::CallbackInfo& info);
	Napi::Value GetWantCaptureKeyboard(const Napi::CallbackInfo& info);
	Napi::Value GetWantTextInput(const Napi::CallbackInfo& info);
	Napi::Value GetWantSetMousePos(const Napi::CallbackInfo& info);
	Napi::Value GetWantSaveIniSettings(const Napi::CallbackInfo& info);
	Napi::Value GetNavActive(const Napi::CallbackInfo& info);
	Napi::Value GetNavVisible(const Napi::CallbackInfo& info);
	Napi::Value GetFramerate(const Napi::CallbackInfo& info);
	Napi::Value GetMetricsRenderVertices(const Napi::CallbackInfo& info);
	Napi::Value GetMetricsRenderIndices(const Napi::CallbackInfo& info);
	Napi::Value GetMetricsRenderWindows(const Napi::CallbackInfo& info);
	Napi::Value GetMetricsActiveWindows(const Napi::CallbackInfo& info);
	Napi::Value GetMetricsActiveAllocations(const Napi::CallbackInfo& info);
	Napi::Value GetMouseDelta(const Napi::CallbackInfo& info);
};

}
