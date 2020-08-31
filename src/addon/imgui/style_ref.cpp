#include "addon/imgui/param.h"
#include "addon/imgui/style_ref.h"
#include "addon/param.h"

using namespace Param;
using namespace Addon::ImGui_::Param;

namespace Addon::ImGui_ {

Napi::FunctionReference StyleRef::constructor;

Napi::Object StyleRef::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "StyleRef", {
		InstanceAccessor<&StyleRef::GetAlpha, &StyleRef::SetAlpha>("alpha"),
		InstanceAccessor<&StyleRef::GetWindowPadding, &StyleRef::SetWindowPadding>("windowPadding"),
		InstanceAccessor<&StyleRef::GetWindowRounding, &StyleRef::SetWindowRounding>("windowRounding"),
		InstanceAccessor<&StyleRef::GetWindowBorderSize, &StyleRef::SetWindowBorderSize>("windowBorderSize"),
		InstanceAccessor<&StyleRef::GetWindowMinSize, &StyleRef::SetWindowMinSize>("windowMinSize"),
		InstanceAccessor<&StyleRef::GetWindowTitleAlign, &StyleRef::SetWindowTitleAlign>("windowTitleAlign"),
		InstanceAccessor<&StyleRef::GetWindowMenuButtonPosition, &StyleRef::SetWindowMenuButtonPosition>("windowMenuButtonPosition"),
		InstanceAccessor<&StyleRef::GetChildRounding, &StyleRef::SetChildRounding>("childRounding"),
		InstanceAccessor<&StyleRef::GetChildBorderSize, &StyleRef::SetChildBorderSize>("childBorderSize"),
		InstanceAccessor<&StyleRef::GetPopupRounding, &StyleRef::SetPopupRounding>("popupRounding"),
		InstanceAccessor<&StyleRef::GetPopupBorderSize, &StyleRef::SetPopupBorderSize>("popupBorderSize"),
		InstanceAccessor<&StyleRef::GetFramePadding, &StyleRef::SetFramePadding>("framePadding"),
		InstanceAccessor<&StyleRef::GetFrameRounding, &StyleRef::SetFrameRounding>("frameRounding"),
		InstanceAccessor<&StyleRef::GetFrameBorderSize, &StyleRef::SetFrameBorderSize>("frameBorderSize"),
		InstanceAccessor<&StyleRef::GetItemSpacing, &StyleRef::SetItemSpacing>("itemSpacing"),
		InstanceAccessor<&StyleRef::GetItemInnerSpacing, &StyleRef::SetItemInnerSpacing>("itemInnerSpacing"),
		InstanceAccessor<&StyleRef::GetTouchExtraPadding, &StyleRef::SetTouchExtraPadding>("touchExtraPadding"),
		InstanceAccessor<&StyleRef::GetIndentSpacing, &StyleRef::SetIndentSpacing>("indentSpacing"),
		InstanceAccessor<&StyleRef::GetColumnsMinSpacing, &StyleRef::SetColumnsMinSpacing>("columnsMinSpacing"),
		InstanceAccessor<&StyleRef::GetScrollbarSize, &StyleRef::SetScrollbarSize>("scrollbarSize"),
		InstanceAccessor<&StyleRef::GetScrollbarRounding, &StyleRef::SetScrollbarRounding>("scrollbarRounding"),
		InstanceAccessor<&StyleRef::GetGrabMinSize, &StyleRef::SetGrabMinSize>("grabMinSize"),
		InstanceAccessor<&StyleRef::GetGrabRounding, &StyleRef::SetGrabRounding>("grabRounding"),
		InstanceAccessor<&StyleRef::GetLogSliderDeadzone, &StyleRef::SetLogSliderDeadzone>("logSliderDeadzone"),
		InstanceAccessor<&StyleRef::GetTabRounding, &StyleRef::SetTabRounding>("tabRounding"),
		InstanceAccessor<&StyleRef::GetTabBorderSize, &StyleRef::SetTabBorderSize>("tabBorderSize"),
		InstanceAccessor<&StyleRef::GetTabMinWidthForUnselectedCloseButton, &StyleRef::SetTabMinWidthForUnselectedCloseButton>("tabMinWidthForUnselectedCloseButton"),
		InstanceAccessor<&StyleRef::GetColorButtonPosition, &StyleRef::SetColorButtonPosition>("colorButtonPosition"),
		InstanceAccessor<&StyleRef::GetButtonTextAlign, &StyleRef::SetButtonTextAlign>("buttonTextAlign"),
		InstanceAccessor<&StyleRef::GetSelectableTextAlign, &StyleRef::SetSelectableTextAlign>("selectableTextAlign"),
		InstanceAccessor<&StyleRef::GetDisplayWindowPadding, &StyleRef::SetDisplayWindowPadding>("displayWindowPadding"),
		InstanceAccessor<&StyleRef::GetDisplaySafeAreaPadding, &StyleRef::SetDisplaySafeAreaPadding>("displaySafeAreaPadding"),
		InstanceAccessor<&StyleRef::GetMouseCursorScale, &StyleRef::SetMouseCursorScale>("mouseCursorScale"),
		InstanceAccessor<&StyleRef::GetAntiAliasedLines, &StyleRef::SetAntiAliasedLines>("antiAliasedLines"),
		InstanceAccessor<&StyleRef::GetAntiAliasedFill, &StyleRef::SetAntiAliasedFill>("antiAliasedFill"),
		InstanceAccessor<&StyleRef::GetCurveTessellationTol, &StyleRef::SetCurveTessellationTol>("curveTessellationTol"),
		InstanceAccessor<&StyleRef::GetCircleSegmentMaxError, &StyleRef::SetCircleSegmentMaxError>("circleSegmentMaxError"),
		InstanceMethod("getColor", &StyleRef::GetColor),
		InstanceMethod("setColor", &StyleRef::SetColor),
		InstanceMethod("scaleAllSizes", &StyleRef::ScaleAllSizes)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("StyleRef", func);

    return exports;
}

Napi::Object StyleRef::Create(Napi::Env env, ImGuiStyle* style)
{
    return constructor.New({FromPtr(env, style)});
}

StyleRef::StyleRef(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<StyleRef>{info}
{
	m_style = static_cast<ImGuiStyle*>(AsPtr(info[0]));
}

Napi::Value StyleRef::GetAlpha(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->Alpha);
}

void StyleRef::SetAlpha(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->Alpha = AsF32(value);
}

Napi::Value StyleRef::GetWindowPadding(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->WindowPadding);
}

void StyleRef::SetWindowPadding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->WindowPadding = AsVec2(value);
}

Napi::Value StyleRef::GetWindowRounding(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->WindowRounding);
}

void StyleRef::SetWindowRounding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->WindowRounding = AsF32(value);
}

Napi::Value StyleRef::GetWindowBorderSize(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->WindowBorderSize);
}

void StyleRef::SetWindowBorderSize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->WindowBorderSize = AsF32(value);
}

Napi::Value StyleRef::GetWindowMinSize(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->WindowMinSize);
}

void StyleRef::SetWindowMinSize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->WindowMinSize = AsVec2(value);
}

Napi::Value StyleRef::GetWindowTitleAlign(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->WindowTitleAlign);
}

void StyleRef::SetWindowTitleAlign(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->WindowTitleAlign = AsVec2(value);
}

Napi::Value StyleRef::GetWindowMenuButtonPosition(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_style->WindowMenuButtonPosition);
}

void StyleRef::SetWindowMenuButtonPosition(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->WindowMenuButtonPosition = AsS32(value);
}

Napi::Value StyleRef::GetChildRounding(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->ChildRounding);
}

void StyleRef::SetChildRounding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->ChildRounding = AsF32(value);
}

Napi::Value StyleRef::GetChildBorderSize(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->ChildBorderSize);
}

void StyleRef::SetChildBorderSize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->ChildBorderSize = AsF32(value);
}

Napi::Value StyleRef::GetPopupRounding(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->PopupRounding);
}

void StyleRef::SetPopupRounding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->PopupRounding = AsF32(value);
}

Napi::Value StyleRef::GetPopupBorderSize(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->PopupBorderSize);
}

void StyleRef::SetPopupBorderSize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->PopupBorderSize = AsF32(value);
}

Napi::Value StyleRef::GetFramePadding(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->FramePadding);
}

void StyleRef::SetFramePadding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->FramePadding = AsVec2(value);
}

Napi::Value StyleRef::GetFrameRounding(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->FrameRounding);
}

void StyleRef::SetFrameRounding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->FrameRounding = AsF32(value);
}

Napi::Value StyleRef::GetFrameBorderSize(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->FrameBorderSize);
}

void StyleRef::SetFrameBorderSize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->FrameBorderSize = AsF32(value);
}

Napi::Value StyleRef::GetItemSpacing(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->ItemSpacing);
}

void StyleRef::SetItemSpacing(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->ItemSpacing = AsVec2(value);
}

Napi::Value StyleRef::GetItemInnerSpacing(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->ItemInnerSpacing);
}

void StyleRef::SetItemInnerSpacing(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->ItemInnerSpacing = AsVec2(value);
}

Napi::Value StyleRef::GetTouchExtraPadding(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->TouchExtraPadding);
}

void StyleRef::SetTouchExtraPadding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->TouchExtraPadding = AsVec2(value);
}

Napi::Value StyleRef::GetIndentSpacing(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->IndentSpacing);
}

void StyleRef::SetIndentSpacing(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->IndentSpacing = AsF32(value);
}

Napi::Value StyleRef::GetColumnsMinSpacing(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->ColumnsMinSpacing);
}

void StyleRef::SetColumnsMinSpacing(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->ColumnsMinSpacing = AsF32(value);
}

Napi::Value StyleRef::GetScrollbarSize(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->ScrollbarSize);
}

void StyleRef::SetScrollbarSize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->ScrollbarSize = AsF32(value);
}

Napi::Value StyleRef::GetScrollbarRounding(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->ScrollbarRounding);
}

void StyleRef::SetScrollbarRounding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->ScrollbarRounding = AsF32(value);
}

Napi::Value StyleRef::GetGrabMinSize(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->GrabMinSize);
}

void StyleRef::SetGrabMinSize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->GrabMinSize = AsF32(value);
}

Napi::Value StyleRef::GetGrabRounding(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->GrabRounding);
}

void StyleRef::SetGrabRounding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->GrabRounding = AsF32(value);
}

Napi::Value StyleRef::GetLogSliderDeadzone(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), m_style->LogSliderDeadzone);
}

void StyleRef::SetLogSliderDeadzone(const Napi::CallbackInfo&, const Napi::Value& value)
{
    m_style->LogSliderDeadzone = AsF32(value);
}

Napi::Value StyleRef::GetTabRounding(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->TabRounding);
}

void StyleRef::SetTabRounding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->TabRounding = AsF32(value);
}

Napi::Value StyleRef::GetTabBorderSize(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->TabBorderSize);
}

void StyleRef::SetTabBorderSize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->TabBorderSize = AsF32(value);
}

Napi::Value StyleRef::GetTabMinWidthForUnselectedCloseButton(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->TabMinWidthForUnselectedCloseButton);
}

void StyleRef::SetTabMinWidthForUnselectedCloseButton(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->TabMinWidthForUnselectedCloseButton = AsF32(value);
}

Napi::Value StyleRef::GetColorButtonPosition(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_style->ColorButtonPosition);
}

void StyleRef::SetColorButtonPosition(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->ColorButtonPosition = AsS32(value);
}

Napi::Value StyleRef::GetButtonTextAlign(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->ButtonTextAlign);
}

void StyleRef::SetButtonTextAlign(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->ButtonTextAlign = AsVec2(value);
}

Napi::Value StyleRef::GetSelectableTextAlign(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->SelectableTextAlign);
}

void StyleRef::SetSelectableTextAlign(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->SelectableTextAlign = AsVec2(value);
}

Napi::Value StyleRef::GetDisplayWindowPadding(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->DisplayWindowPadding);
}

void StyleRef::SetDisplayWindowPadding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->DisplayWindowPadding = AsVec2(value);
}

Napi::Value StyleRef::GetDisplaySafeAreaPadding(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_style->DisplaySafeAreaPadding);
}

void StyleRef::SetDisplaySafeAreaPadding(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->DisplaySafeAreaPadding = AsVec2(value);
}

Napi::Value StyleRef::GetMouseCursorScale(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->MouseCursorScale);
}

void StyleRef::SetMouseCursorScale(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->MouseCursorScale = AsF32(value);
}

Napi::Value StyleRef::GetAntiAliasedLines(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_style->AntiAliasedLines);
}

void StyleRef::SetAntiAliasedLines(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->AntiAliasedLines = AsBool(value);
}

Napi::Value StyleRef::GetAntiAliasedFill(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_style->AntiAliasedFill);
}

void StyleRef::SetAntiAliasedFill(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->AntiAliasedFill = AsBool(value);
}

Napi::Value StyleRef::GetCurveTessellationTol(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->CurveTessellationTol);
}

void StyleRef::SetCurveTessellationTol(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->CurveTessellationTol = AsF32(value);
}

Napi::Value StyleRef::GetCircleSegmentMaxError(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_style->CircleSegmentMaxError);
}

void StyleRef::SetCircleSegmentMaxError(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_style->CircleSegmentMaxError = AsF32(value);
}

Napi::Value StyleRef::GetColor(const Napi::CallbackInfo& info)
{
	return FromColor(info.Env(), m_style->Colors[AsS32(info[0])]);
}

Napi::Value StyleRef::SetColor(const Napi::CallbackInfo& info)
{
	m_style->Colors[AsS32(info[0])] = AsColor(info[1]);

	return info.Env().Undefined();
}

Napi::Value StyleRef::ScaleAllSizes(const Napi::CallbackInfo& info)
{
	m_style->ScaleAllSizes(AsF32(info[0]));

	return info.Env().Undefined();
}

}
