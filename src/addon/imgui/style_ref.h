#pragma once

#include "imgui/imgui.h"

#include <napi.h>

namespace Addon::ImGui_ {

struct StyleRef : Napi::ObjectWrap<StyleRef> {
    static Napi::FunctionReference constructor;
	ImGuiStyle* m_style;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
	static Napi::Object Create(Napi::Env env, ImGuiStyle* style);

    StyleRef(const Napi::CallbackInfo& info);

	Napi::Value GetAlpha(const Napi::CallbackInfo& info);
	void SetAlpha(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetWindowPadding(const Napi::CallbackInfo& info);
	void SetWindowPadding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetWindowRounding(const Napi::CallbackInfo& info);
	void SetWindowRounding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetWindowBorderSize(const Napi::CallbackInfo& info);
	void SetWindowBorderSize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetWindowMinSize(const Napi::CallbackInfo& info);
	void SetWindowMinSize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetWindowTitleAlign(const Napi::CallbackInfo& info);
	void SetWindowTitleAlign(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetWindowMenuButtonPosition(const Napi::CallbackInfo& info);
	void SetWindowMenuButtonPosition(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetChildRounding(const Napi::CallbackInfo& info);
	void SetChildRounding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetChildBorderSize(const Napi::CallbackInfo& info);
	void SetChildBorderSize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetPopupRounding(const Napi::CallbackInfo& info);
	void SetPopupRounding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetPopupBorderSize(const Napi::CallbackInfo& info);
	void SetPopupBorderSize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetFramePadding(const Napi::CallbackInfo& info);
	void SetFramePadding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetFrameRounding(const Napi::CallbackInfo& info);
	void SetFrameRounding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetFrameBorderSize(const Napi::CallbackInfo& info);
	void SetFrameBorderSize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetItemSpacing(const Napi::CallbackInfo& info);
	void SetItemSpacing(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetItemInnerSpacing(const Napi::CallbackInfo& info);
	void SetItemInnerSpacing(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetTouchExtraPadding(const Napi::CallbackInfo& info);
	void SetTouchExtraPadding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetIndentSpacing(const Napi::CallbackInfo& info);
	void SetIndentSpacing(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetColumnsMinSpacing(const Napi::CallbackInfo& info);
	void SetColumnsMinSpacing(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetScrollbarSize(const Napi::CallbackInfo& info);
	void SetScrollbarSize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetScrollbarRounding(const Napi::CallbackInfo& info);
	void SetScrollbarRounding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetGrabMinSize(const Napi::CallbackInfo& info);
	void SetGrabMinSize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetGrabRounding(const Napi::CallbackInfo& info);
	void SetGrabRounding(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value GetLogSliderDeadzone(const Napi::CallbackInfo& info);
	void SetLogSliderDeadzone(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetTabRounding(const Napi::CallbackInfo& info);
	void SetTabRounding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetTabBorderSize(const Napi::CallbackInfo& info);
	void SetTabBorderSize(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetTabMinWidthForUnselectedCloseButton(const Napi::CallbackInfo& info);
	void SetTabMinWidthForUnselectedCloseButton(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetColorButtonPosition(const Napi::CallbackInfo& info);
	void SetColorButtonPosition(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetButtonTextAlign(const Napi::CallbackInfo& info);
	void SetButtonTextAlign(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetSelectableTextAlign(const Napi::CallbackInfo& info);
	void SetSelectableTextAlign(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetDisplayWindowPadding(const Napi::CallbackInfo& info);
	void SetDisplayWindowPadding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetDisplaySafeAreaPadding(const Napi::CallbackInfo& info);
	void SetDisplaySafeAreaPadding(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetMouseCursorScale(const Napi::CallbackInfo& info);
	void SetMouseCursorScale(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetAntiAliasedLines(const Napi::CallbackInfo& info);
	void SetAntiAliasedLines(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetAntiAliasedFill(const Napi::CallbackInfo& info);
	void SetAntiAliasedFill(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetCurveTessellationTol(const Napi::CallbackInfo& info);
	void SetCurveTessellationTol(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetCircleSegmentMaxError(const Napi::CallbackInfo& info);
	void SetCircleSegmentMaxError(const Napi::CallbackInfo& info, const Napi::Value& value);
	Napi::Value GetColor(const Napi::CallbackInfo& info);
	Napi::Value SetColor(const Napi::CallbackInfo& info);
	Napi::Value ScaleAllSizes(const Napi::CallbackInfo& info);
};

}
