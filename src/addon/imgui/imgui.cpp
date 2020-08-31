#include "addon/imgui/draw_list_ref.h"
#include "addon/imgui/font_atlas_ref.h"
#include "addon/imgui/font_ref.h"
#include "addon/imgui/io_ref.h"
#include "addon/imgui/param.h"
#include "addon/imgui/style_ref.h"
#include "addon/imgui/viewport_ref.h"
#include "addon/param.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"

using namespace Param;
using namespace Addon::ImGui_::Param;

namespace Addon::ImGui_ {

Napi::Value GetIO(const Napi::CallbackInfo& info)
{
    return IORef::Create(info.Env(), &ImGui::GetIO());
}

Napi::Value GetStyle(const Napi::CallbackInfo& info)
{
    return StyleRef::Create(info.Env(), &ImGui::GetStyle());
}

Napi::Value ShowDemoWindow(const Napi::CallbackInfo& info)
{
    auto a_open = AsArray(info[0]);

    if (a_open.IsUndefined())
        ImGui::ShowDemoWindow();
    else {
        auto open = AsBool(a_open.Get(0u));

        ImGui::ShowDemoWindow(&open);

        a_open.Set(0u, FromBool(info.Env(), open));
    }

    return info.Env().Undefined();
}

Napi::Value ShowAboutWindow(const Napi::CallbackInfo& info)
{
    auto a_open = AsArray(info[0]);

    if (a_open.IsUndefined())
        ImGui::ShowAboutWindow();
    else {
        auto open = AsBool(a_open.Get(0u));

        ImGui::ShowAboutWindow(&open);

        a_open.Set(0u, FromBool(info.Env(), open));
    }

    return info.Env().Undefined();
}

Napi::Value ShowMetricsWindow(const Napi::CallbackInfo& info)
{
    auto a_open = AsArray(info[0]);

    if (a_open.IsUndefined())
        ImGui::ShowMetricsWindow();
    else {
        auto open = AsBool(a_open.Get(0u));

        ImGui::ShowMetricsWindow(&open);

        a_open.Set(0u, FromBool(info.Env(), open));
    }

    return info.Env().Undefined();
}

Napi::Value ShowStyleEditor(const Napi::CallbackInfo& info)
{
    ImGui::ShowStyleEditor();

    return info.Env().Undefined();
}

Napi::Value ShowStyleSelector(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::ShowStyleSelector(GetStrUtf8(0)));
}

Napi::Value ShowFontSelector(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::ShowFontSelector(GetStrUtf8(0));

    return info.Env().Undefined();
}

Napi::Value ShowUserGuide(const Napi::CallbackInfo& info)
{
    ImGui::ShowUserGuide();

    return info.Env().Undefined();
}

Napi::Value GetVersion(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), ImGui::GetVersion());
}

Napi::Value StyleColorsDark(const Napi::CallbackInfo& info)
{
    ImGui::StyleColorsDark();

    return info.Env().Undefined();
}

Napi::Value StyleColorsClassic(const Napi::CallbackInfo& info)
{
    ImGui::StyleColorsClassic();

    return info.Env().Undefined();
}

Napi::Value StyleColorsLight(const Napi::CallbackInfo& info)
{
    ImGui::StyleColorsLight();

    return info.Env().Undefined();
}

Napi::Value Begin(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto a_open = AsArray(info[1]);
    bool ret{};

    if (a_open.IsUndefined())
        ret = ImGui::Begin(GetStrUtf8(0), nullptr, AsS32Or(info[2], 0));
    else {
        auto open = AsBool(a_open.Get(0u));

        ret = ImGui::Begin(GetStrUtf8(0), &open, AsS32Or(info[2], 0));

        a_open.Set(0u, FromBool(info.Env(), open));
    }

    return FromBool(info.Env(), ret);
}

Napi::Value End(const Napi::CallbackInfo& info)
{
    ImGui::End();

    return info.Env().Undefined();
}

Napi::Value BeginChild(const Napi::CallbackInfo& info)
{
    bool ret{};

    if (info[0].IsString()) {
        LoadStrUtf8(0, info[0]);
        ret = ImGui::BeginChild(GetStrUtf8(0), AsVec2Or(info[1], {}), AsBoolOr(info[2], false),
            AsS32Or(info[3], 0));
    }
    else if (info[0].IsNumber()) {
        ret = ImGui::BeginChild(AsU32(info[0]), AsVec2Or(info[1], {}), AsBoolOr(info[2], false),
            AsS32Or(info[3], 0));
    }

    return FromBool(info.Env(), ret);
}

Napi::Value EndChild(const Napi::CallbackInfo& info)
{
    ImGui::EndChild();

    return info.Env().Undefined();
}

Napi::Value IsWindowAppearing(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsWindowAppearing());
}

Napi::Value IsWindowCollapsed(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsWindowCollapsed());
}

Napi::Value IsWindowFocused(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsWindowFocused(AsS32Or(info[0], 0)));
}

Napi::Value IsWindowHovered(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsWindowHovered(AsS32Or(info[0], 0)));
}

Napi::Value GetWindowDrawList(const Napi::CallbackInfo& info)
{
    return DrawListRef::Create(info.Env(), ImGui::GetWindowDrawList(), ImGui::GetWindowPos());
}

Napi::Value GetWindowDPIScale(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetWindowDpiScale());
}

Napi::Value GetWindowViewport(const Napi::CallbackInfo& info)
{
    return ViewportRef::Create(info.Env(), ImGui::GetWindowViewport());
}

Napi::Value GetWindowPos(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetWindowPos());
}

Napi::Value GetWindowSize(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetWindowSize());
}

Napi::Value GetWindowWidth(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetWindowWidth());
}

Napi::Value GetWindowHeight(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetWindowHeight());
}

Napi::Value SetNextWindowPos(const Napi::CallbackInfo& info)
{
    ImGui::SetNextWindowPos(AsVec2(info[0]), AsS32Or(info[1], 0), AsVec2Or(info[2], {}));

    return info.Env().Undefined();
}

Napi::Value SetNextWindowSize(const Napi::CallbackInfo& info)
{
    ImGui::SetNextWindowSize(AsVec2(info[0]), AsS32Or(info[1], 0));

    return info.Env().Undefined();
}

Napi::Value SetNextWindowSizeConstraints(const Napi::CallbackInfo& info)
{
    ImGui::SetNextWindowSizeConstraints(AsVec2(info[0]), AsVec2(info[1]));

    return info.Env().Undefined();
}

Napi::Value SetNextWindowContentSize(const Napi::CallbackInfo& info)
{
    ImGui::SetNextWindowContentSize(AsVec2(info[0]));

    return info.Env().Undefined();
}

Napi::Value SetNextWindowCollapsed(const Napi::CallbackInfo& info)
{
    ImGui::SetNextWindowCollapsed(AsBool(info[0]), AsS32Or(info[1], 0));

    return info.Env().Undefined();
}

Napi::Value SetNextWindowFocus(const Napi::CallbackInfo& info)
{
    ImGui::SetNextWindowFocus();

    return info.Env().Undefined();
}

Napi::Value SetNextWindowBgAlpha(const Napi::CallbackInfo& info)
{
    ImGui::SetNextWindowBgAlpha(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value SetNextWindowViewport(const Napi::CallbackInfo& info)
{
    ImGui::SetNextWindowViewport(AsU32(info[0]));

    return info.Env().Undefined();
}

Napi::Value SetWindowPos(const Napi::CallbackInfo& info)
{
    if (info[0].IsString()) {
        LoadStrUtf8(0, info[0]);
        ImGui::SetWindowPos(GetStrUtf8(0), AsVec2(info[1]), AsS32Or(info[2], 0));
    }
    else {
        ImGui::SetWindowPos(AsVec2(info[0]), AsS32Or(info[1], 0));
    }

    return info.Env().Undefined();
}

Napi::Value SetWindowSize(const Napi::CallbackInfo& info)
{
    if (info[0].IsString()) {
        LoadStrUtf8(0, info[0]);
        ImGui::SetWindowSize(GetStrUtf8(0), AsVec2(info[1]), AsS32Or(info[2], 0));
    }
    else {
        ImGui::SetWindowSize(AsVec2(info[0]), AsS32Or(info[1], 0));
    }

    return info.Env().Undefined();
}

Napi::Value SetWindowCollapsed(const Napi::CallbackInfo& info)
{
    if (info[0].IsString()) {
        LoadStrUtf8(0, info[0]);
        ImGui::SetWindowCollapsed(GetStrUtf8(0), AsBool(info[1]), AsS32Or(info[2], 0));
    }
    else {
        ImGui::SetWindowCollapsed(AsBool(info[0]), AsS32Or(info[1], 0));
    }

    return info.Env().Undefined();
}

Napi::Value SetWindowFocus(const Napi::CallbackInfo& info)
{
    if (info[0].IsString()) {
        LoadStrUtf8(0, info[0]);
        ImGui::SetWindowFocus(GetStrUtf8(0));
    }
    else {
        ImGui::SetWindowFocus();
    }

    return info.Env().Undefined();
}

Napi::Value SetWindowFontScale(const Napi::CallbackInfo& info)
{
    ImGui::SetWindowFontScale(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value GetContentRegionMax(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetContentRegionMax());
}

Napi::Value GetContentRegionAvail(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetContentRegionAvail());
}

Napi::Value GetWindowContentRegionMin(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetWindowContentRegionMin());
}

Napi::Value GetWindowContentRegionMax(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetWindowContentRegionMax());
}

Napi::Value GetWindowContentRegionWidth(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetWindowContentRegionWidth());
}

Napi::Value GetScrollX(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetScrollX());
}

Napi::Value GetScrollY(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetScrollY());
}

Napi::Value GetScrollMaxX(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetScrollMaxX());
}

Napi::Value GetScrollMaxY(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetScrollMaxY());
}

Napi::Value SetScrollX(const Napi::CallbackInfo& info)
{
    ImGui::SetScrollX(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value SetScrollY(const Napi::CallbackInfo& info)
{
    ImGui::SetScrollY(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value SetScrollHereX(const Napi::CallbackInfo& info)
{
    ImGui::SetScrollHereX(AsF32Or(info[0], 0.5f));

    return info.Env().Undefined();
}

Napi::Value SetScrollHereY(const Napi::CallbackInfo& info)
{
    ImGui::SetScrollHereY(AsF32Or(info[0], 0.5f));

    return info.Env().Undefined();
}

Napi::Value SetScrollFromPosX(const Napi::CallbackInfo& info)
{
    ImGui::SetScrollFromPosX(AsF32(info[0]), AsF32Or(info[1], 0.5f));

    return info.Env().Undefined();
}

Napi::Value SetScrollFromPosY(const Napi::CallbackInfo& info)
{
    ImGui::SetScrollFromPosY(AsF32(info[0]), AsF32Or(info[1], 0.5f));

    return info.Env().Undefined();
}

Napi::Value PushFont(const Napi::CallbackInfo& info)
{
    ImGui::PushFont(AsFontPtrOr(info[0], nullptr));

    return info.Env().Undefined();
}

Napi::Value PopFont(const Napi::CallbackInfo& info)
{
    ImGui::PopFont();

    return info.Env().Undefined();
}

Napi::Value PushStyleColor(const Napi::CallbackInfo& info)
{
    ImGui::PushStyleColor(AsS32(info[0]), AsU32Color(info[1]));

    return info.Env().Undefined();
}

Napi::Value PopStyleColor(const Napi::CallbackInfo& info)
{
    ImGui::PopStyleColor(AsS32Or(info[0], 1));

    return info.Env().Undefined();
}

Napi::Value PushStyleVar(const Napi::CallbackInfo& info)
{
    if (info[1].IsNumber())
        ImGui::PushStyleVar(AsS32(info[0]), AsF32(info[1]));
    else if (info[1].IsObject())
        ImGui::PushStyleVar(AsS32(info[0]), AsVec2(info[1]));

    return info.Env().Undefined();
}

Napi::Value PopStyleVar(const Napi::CallbackInfo& info)
{
    ImGui::PopStyleVar(AsS32Or(info[0], 1));

    return info.Env().Undefined();
}

Napi::Value GetStyleColor(const Napi::CallbackInfo& info)
{
    return FromColor(info.Env(), ImGui::GetStyleColorVec4(AsS32(info[0])));
}

Napi::Value GetFont(const Napi::CallbackInfo& info)
{
    return FontRef::Create(info.Env(), ImGui::GetFont());
}

Napi::Value GetFontSize(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetFontSize());
}

Napi::Value GetFontTexUvWhitePixel(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetFontTexUvWhitePixel());
}

Napi::Value GetColor(const Napi::CallbackInfo& info)
{
    ImU32 ret{};

    if (info[0].IsNumber())
        ret = ImGui::GetColorU32(AsS32(info[0]), AsF32Or(info[1], 1.0f));
    else if (info[0].IsObject())
        ret = ImGui::GetColorU32(AsColor(info[0]).Value);

    return FromColor(info.Env(), ret);
}

Napi::Value PushItemWidth(const Napi::CallbackInfo& info)
{
    ImGui::PushItemWidth(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value PopItemWidth(const Napi::CallbackInfo& info)
{
    ImGui::PopItemWidth();

    return info.Env().Undefined();
}

Napi::Value SetNextItemWidth(const Napi::CallbackInfo& info)
{
    ImGui::SetNextItemWidth(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value CalcItemWidth(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::CalcItemWidth());
}

Napi::Value PushTextWrapPos(const Napi::CallbackInfo& info)
{
    ImGui::PushTextWrapPos(AsF32Or(info[0], 0.0f));

    return info.Env().Undefined();
}

Napi::Value PopTextWrapPos(const Napi::CallbackInfo& info)
{
    ImGui::PopTextWrapPos();

    return info.Env().Undefined();
}

Napi::Value PushAllowKeyboardFocus(const Napi::CallbackInfo& info)
{
    ImGui::PushAllowKeyboardFocus(AsBool(info[0]));

    return info.Env().Undefined();
}

Napi::Value PopAllowKeyboardFocus(const Napi::CallbackInfo& info)
{
    ImGui::PopAllowKeyboardFocus();

    return info.Env().Undefined();
}

Napi::Value PushButtonRepeat(const Napi::CallbackInfo& info)
{
    ImGui::PushButtonRepeat(AsBool(info[0]));

    return info.Env().Undefined();
}

Napi::Value PopButtonRepeat(const Napi::CallbackInfo& info)
{
    ImGui::PopButtonRepeat();

    return info.Env().Undefined();
}

Napi::Value Separator(const Napi::CallbackInfo& info)
{
    ImGui::Separator();

    return info.Env().Undefined();
}

Napi::Value SameLine(const Napi::CallbackInfo& info)
{
    ImGui::SameLine(AsF32Or(info[0], 0.0f), AsF32Or(info[1], -1.0f));

    return info.Env().Undefined();
}

Napi::Value NewLine(const Napi::CallbackInfo& info)
{
    ImGui::NewLine();

    return info.Env().Undefined();
}

Napi::Value Spacing(const Napi::CallbackInfo& info)
{
    ImGui::Spacing();

    return info.Env().Undefined();
}

Napi::Value Dummy(const Napi::CallbackInfo& info)
{
    ImGui::Dummy(AsVec2(info[0]));

    return info.Env().Undefined();
}

Napi::Value Indent(const Napi::CallbackInfo& info)
{
    ImGui::Indent(AsF32Or(info[0], 0.0f));

    return info.Env().Undefined();
}

Napi::Value Unindent(const Napi::CallbackInfo& info)
{
    ImGui::Unindent(AsF32Or(info[0], 0.0f));

    return info.Env().Undefined();
}

Napi::Value BeginGroup(const Napi::CallbackInfo& info)
{
    ImGui::BeginGroup();

    return info.Env().Undefined();
}

Napi::Value EndGroup(const Napi::CallbackInfo& info)
{
    ImGui::EndGroup();

    return info.Env().Undefined();
}

Napi::Value GetCursorPos(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetCursorPos());
}

Napi::Value GetCursorPosX(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetCursorPosX());
}

Napi::Value GetCursorPosY(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetCursorPosY());
}

Napi::Value SetCursorPos(const Napi::CallbackInfo& info)
{
    ImGui::SetCursorPos(AsVec2(info[0]));

    return info.Env().Undefined();
}

Napi::Value SetCursorPosX(const Napi::CallbackInfo& info)
{
    ImGui::SetCursorPosX(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value SetCursorPosY(const Napi::CallbackInfo& info)
{
    ImGui::SetCursorPosY(AsF32(info[0]));

    return info.Env().Undefined();
}

Napi::Value GetCursorStartPos(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetCursorStartPos());
}

Napi::Value GetCursorScreenPos(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetCursorScreenPos());
}

Napi::Value SetCursorScreenPos(const Napi::CallbackInfo& info)
{
    ImGui::SetCursorScreenPos(AsVec2(info[0]));

    return info.Env().Undefined();
}

Napi::Value AlignTextToFramePadding(const Napi::CallbackInfo& info)
{
    ImGui::AlignTextToFramePadding();

    return info.Env().Undefined();
}

Napi::Value GetTextLineHeight(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetTextLineHeight());
}

Napi::Value GetTextLineHeightWithSpacing(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetTextLineHeightWithSpacing());
}

Napi::Value GetFrameHeight(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetFrameHeight());
}

Napi::Value GetFrameHeightWithSpacing(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetFrameHeightWithSpacing());
}

Napi::Value PushId(const Napi::CallbackInfo& info)
{
    if (info[0].IsString()) {
        LoadStrUtf8(0, info[0]);
        ImGui::PushID(GetStrUtf8(0));
    }
    else if (info[0].IsNumber()) {
        ImGui::PushID(AsPtr(info[0]));
    }

    return info.Env().Undefined();
}

Napi::Value PopId(const Napi::CallbackInfo& info)
{
    ImGui::PopID();

    return info.Env().Undefined();
}

Napi::Value GetId(const Napi::CallbackInfo& info)
{
    ImGuiID ret{};

    if (info[0].IsString()) {
        LoadStrUtf8(0, info[0]);
        ret = ImGui::GetID(GetStrUtf8(0));
    }
    else if (info[0].IsNumber()) {
        ret = ImGui::GetID(AsPtr(info[0]));
    }

    return FromU32(info.Env(), ret);
}

Napi::Value Text(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::TextUnformatted(GetStrUtf8(0));

    return info.Env().Undefined();
}

Napi::Value TextColored(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::TextColored(AsColor(info[1]).Value, "%s", GetStrUtf8(0));

    return info.Env().Undefined();
}

Napi::Value TextDisabled(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::TextDisabled("%s", GetStrUtf8(0));

    return info.Env().Undefined();
}

Napi::Value TextWrapped(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::TextWrapped("%s", GetStrUtf8(0));

    return info.Env().Undefined();
}

Napi::Value LabelText(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8(1, info[1]);
    ImGui::LabelText(GetStrUtf8(0), "%s", GetStrUtf8(1));

    return info.Env().Undefined();
}

Napi::Value BulletText(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::BulletText("%s", GetStrUtf8(0));

    return info.Env().Undefined();
}

Napi::Value Button(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::Button(GetStrUtf8(0), AsVec2Or(info[1], {})));
}

Napi::Value SmallButton(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::SmallButton(GetStrUtf8(0)));
}

Napi::Value InvisibleButton(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::InvisibleButton(GetStrUtf8(0), AsVec2(info[1]), AsS32Or(info[2], 0)));
}

Napi::Value ArrowButton(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::ArrowButton(GetStrUtf8(0), AsS32(info[1])));
}

Napi::Value Image(const Napi::CallbackInfo& info)
{
    ImGui::Image(static_cast<ImTextureID>(AsPtr(info[0])), AsVec2(info[1]),
        AsVec2Or(info[2], {0.0f, 0.0f}), AsVec2Or(info[3], {1.0f, 1.0f}),
        AsColorOr(info[4], {1.0f, 1.0f, 1.0f, 1.0f}), AsColorOr(info[5], {}));

    return info.Env().Undefined();
}

Napi::Value ImageButton(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::ImageButton(static_cast<ImTextureID>(AsPtr(info[0])),
        AsVec2(info[1]), AsVec2Or(info[2], {0.0f, 0.0f}), AsVec2Or(info[3], {1.0f, 1.0f}),
        AsS32Or(info[4], -1), AsColorOr(info[5], {}), AsColorOr(info[6], {1.0f, 1.0f, 1.0f, 1.0f})));
}

Napi::Value Checkbox(const Napi::CallbackInfo& info)
{
    auto a_v = AsArray(info[1]);
    bool ret{};

    auto v = AsBool(a_v.Get(0u));

    LoadStrUtf8(0, info[0]);
    ret = ImGui::Checkbox(GetStrUtf8(0), &v);

    a_v.Set(0u, FromBool(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value CheckboxFlags(const Napi::CallbackInfo& info)
{
    auto a_flags = AsArray(info[1]);
    bool ret{};

    auto flags = AsU32(a_flags.Get(0u));

    LoadStrUtf8(0, info[0]);
    ret = ImGui::CheckboxFlags(GetStrUtf8(0), &flags, AsU32(info[2]));

    a_flags.Set(0u, FromU32(info.Env(), flags));

    return FromBool(info.Env(), ret);
}

Napi::Value RadioButton(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    if (info.Length() == 2) {
        return FromBool(info.Env(), ImGui::RadioButton(GetStrUtf8(0), AsBool(info[1])));
    }
    else if (info.Length() == 3) {
        auto a_v = AsArray(info[1]);
        bool ret{};

        auto v = AsS32(a_v.Get(0u));

        ret = ImGui::RadioButton(GetStrUtf8(0), &v, AsS32(info[2]));

        a_v.Set(0u, FromS32(info.Env(), v));

        return FromBool(info.Env(), ret);
    }

    return info.Env().Undefined();
}

Napi::Value ProgressBar(const Napi::CallbackInfo& info)
{
    if (info[2].IsUndefined()) {
        ImGui::ProgressBar(AsF32(info[0]), AsVec2Or(info[1], {-1.0f, 0.0f}));
    }
    else {
        LoadStrUtf8(0, info[2]);
        ImGui::ProgressBar(AsF32(info[0]), AsVec2Or(info[1], {-1.0f, 0.0f}), GetStrUtf8(0));
    }

    return info.Env().Undefined();
}

Napi::Value Bullet(const Napi::CallbackInfo& info)
{
    ImGui::Bullet();

    return info.Env().Undefined();
}

Napi::Value BeginCombo(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8(1, info[1]);

    return FromBool(info.Env(), ImGui::BeginCombo(GetStrUtf8(0), GetStrUtf8(1),
        AsS32Or(info[2], 0)));
}

Napi::Value EndCombo(const Napi::CallbackInfo& info)
{
    ImGui::EndCombo();

    return info.Env().Undefined();
}

Napi::Value Combo(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto a_current_item = AsArray(info[1]);
    auto items = AsArray(info[2]);
    bool ret{};

    auto current_item = AsS32(a_current_item.Get(0u));

    const auto func = [](void* data, int idx, const char** out_text) {
        static char str[512];
        auto real_data = static_cast<Napi::Array*>(data);
        std::strcpy(str, real_data->Get(idx).ToString().Utf8Value().c_str());
        *out_text = str;
        return static_cast<unsigned>(idx) < real_data->Length();
    };

    ret = ImGui::Combo(GetStrUtf8(0), &current_item, func, &items, items.Length(),
        AsS32Or(info[3], -1));

    a_current_item.Set(0u, FromS32(info.Env(), current_item));

    return FromBool(info.Env(), ret);
}

Napi::Value DragFloat(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[5], "%.3f");

    auto a_v = AsArray(info[1]);
    bool ret{};

    auto v = AsF32(a_v.Get(0u));

    ret = ImGui::DragFloat(GetStrUtf8(0), &v, AsF32Or(info[2], 1.0f), AsF32Or(info[3], 0.0f),
        AsF32Or(info[4], 0.0f), GetStrUtf8(1), AsS32Or(info[6], 0));

    a_v.Set(0u, FromF32(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value DragFloats(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[5], "%.3f");

    auto v = AsArray(info[1]);
    auto v_min = AsF32Or(info[3], 0.0f);
    auto v_max = AsF32Or(info[4], 0.0f);

    std::vector<float> floats(v.Length());
    bool ret{};

    for (std::size_t i{}; i < floats.size(); ++i)
        floats[i] = AsF32(v.Get(static_cast<u32>(i)));

    ret = ImGui::DragScalarN(GetStrUtf8(0), ImGuiDataType_Float, floats.data(), static_cast<int>(floats.size()),
        AsF32Or(info[2], 1.0f), &v_min, &v_max, GetStrUtf8(1), AsS32Or(info[6], 0));

    for (std::size_t i{}; i < floats.size(); ++i)
        v.Set(static_cast<u32>(i), FromF32(info.Env(), floats[i]));

    return FromBool(info.Env(), ret);
}

Napi::Value DragFloatRange(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[6], "%.3f");
    LoadStrUtf8Or(2, info[7], "");

    auto a_v_current_min = AsArray(info[1]);
    auto a_v_current_max = AsArray(info[2]);
    bool ret{};

    auto v_current_min = AsF32(a_v_current_min.Get(0u));
    auto v_current_max = AsF32(a_v_current_max.Get(0u));

    ret = ImGui::DragFloatRange2(GetStrUtf8(0), &v_current_min, &v_current_max, AsF32Or(info[3], 1.0f),
        AsF32Or(info[4], 0.0f), AsF32Or(info[5], 0.0f), GetStrUtf8(1),
        GetStrUtf8(2)[0] == '\0' ? nullptr : GetStrUtf8(2), AsS32Or(info[8], 0));

    a_v_current_min.Set(0u, FromF32(info.Env(), v_current_min));
    a_v_current_max.Set(0u, FromF32(info.Env(), v_current_max));

    return FromBool(info.Env(), ret);
}

Napi::Value DragInt(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[5], "%d");

    auto a_v = AsArray(info[1]);
    bool ret{};

    auto v = AsS32(a_v.Get(0u));

    ret = ImGui::DragInt(GetStrUtf8(0), &v, AsF32Or(info[2], 1.0f), AsS32Or(info[3], 0),
        AsS32Or(info[4], 0), GetStrUtf8(1), AsS32Or(info[6], 0));

    a_v.Set(0u, FromS32(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value DragInts(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[5], "%d");

    auto v = AsArray(info[1]);
    auto v_min = AsS32Or(info[3], 0);
    auto v_max = AsS32Or(info[4], 0);

    std::vector<int> ints(v.Length());
    bool ret{};

    for (std::size_t i{}; i < ints.size(); ++i)
        ints[i] = AsS32(v.Get(static_cast<u32>(i)));

    ret = ImGui::DragScalarN(GetStrUtf8(0), ImGuiDataType_S32, ints.data(), static_cast<int>(ints.size()),
        AsF32Or(info[2], 1.0f), &v_min, &v_max, GetStrUtf8(1), AsS32Or(info[6], 0));

    for (std::size_t i{}; i < ints.size(); ++i)
        v.Set(static_cast<u32>(i), FromS32(info.Env(), ints[i]));

    return FromBool(info.Env(), ret);
}

Napi::Value DragIntRange(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[6], "%d");
    LoadStrUtf8Or(2, info[7], "");

    auto a_v_current_min = AsArray(info[1]);
    auto a_v_current_max = AsArray(info[2]);
    bool ret{};

    auto v_current_min = AsS32(a_v_current_min.Get(0u));
    auto v_current_max = AsS32(a_v_current_max.Get(0u));

    ret = ImGui::DragIntRange2(GetStrUtf8(0), &v_current_min, &v_current_max, AsF32Or(info[3], 1.0f),
        AsS32Or(info[4], 0), AsS32Or(info[5], 0), GetStrUtf8(1),
        GetStrUtf8(2)[0] == '\0' ? nullptr : GetStrUtf8(2),
        AsS32Or(info[8], 0));

    a_v_current_min.Set(0u, FromS32(info.Env(), v_current_min));
    a_v_current_max.Set(0u, FromS32(info.Env(), v_current_max));

    return FromBool(info.Env(), ret);
}

Napi::Value SliderFloat(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[4], "%.3f");

    auto a_v = AsArray(info[1]);
    bool ret{};

    auto v = AsF32(a_v.Get(0u));

    ret = ImGui::SliderFloat(GetStrUtf8(0), &v, AsF32(info[2]), AsF32(info[3]), GetStrUtf8(1),
        AsS32Or(info[5], 0));

    a_v.Set(0u, FromF32(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value SliderFloats(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[4], "%.3f");

    auto v = AsArray(info[1]);
    auto v_min = AsF32(info[2]);
    auto v_max = AsF32(info[3]);

    std::vector<float> floats(v.Length());
    bool ret{};

    for (std::size_t i{}; i < floats.size(); ++i)
        floats[i] = AsF32(v.Get(static_cast<u32>(i)));

    ret = ImGui::SliderScalarN(GetStrUtf8(0), ImGuiDataType_Float, floats.data(), static_cast<int>(floats.size()),
        &v_min, &v_max, GetStrUtf8(1), AsS32Or(info[5], 0));

    for (std::size_t i{}; i < floats.size(); ++i)
        v.Set(static_cast<u32>(i), FromF32(info.Env(), floats[i]));

    return FromBool(info.Env(), ret);
}

Napi::Value SliderAngle(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[4], "%.0f deg");

    auto a_v_rad = AsArray(info[1]);
    bool ret{};

    auto v_rad = AsF32(a_v_rad.Get(0u));

    ret = ImGui::SliderAngle(GetStrUtf8(0), &v_rad, AsF32Or(info[2], -360.0f),
        AsF32Or(info[3], 360.0f), GetStrUtf8(1), AsS32Or(info[5], 0));

    a_v_rad.Set(0u, FromF32(info.Env(), v_rad));

    return FromBool(info.Env(), ret);
}

Napi::Value SliderInt(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[4], "%d");

    auto a_v = AsArray(info[1]);
    bool ret{};

    auto v = AsS32(a_v.Get(0u));

    ret = ImGui::SliderInt(GetStrUtf8(0), &v, AsS32(info[2]), AsS32(info[3]), GetStrUtf8(1),
        AsS32Or(info[5], 0));

    a_v.Set(0u, FromS32(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value SliderInts(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[4], "%d");

    auto v = AsArray(info[1]);
    auto v_min = AsS32(info[2]);
    auto v_max = AsS32(info[3]);

    std::vector<int> ints(v.Length());
    bool ret{};

    for (std::size_t i{}; i < ints.size(); ++i)
        ints[i] = AsS32(v.Get(static_cast<u32>(i)));

    ret = ImGui::SliderScalarN(GetStrUtf8(0), ImGuiDataType_S32, ints.data(), static_cast<int>(ints.size()),
        &v_min, &v_max, GetStrUtf8(1), AsS32Or(info[5], 0));

    for (std::size_t i{}; i < ints.size(); ++i)
        v.Set(static_cast<u32>(i), FromS32(info.Env(), ints[i]));

    return FromBool(info.Env(), ret);
}

Napi::Value VSliderFloat(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[5], "%.3f");

    auto a_v = AsArray(info[2]);
    bool ret{};

    auto v = AsF32(a_v.Get(0u));

    ret = ImGui::VSliderFloat(GetStrUtf8(0), AsVec2(info[1]), &v, AsF32(info[3]), AsF32(info[4]),
        GetStrUtf8(1), AsS32Or(info[6], 0));

    a_v.Set(0u, FromF32(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value VSliderInt(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[5], "%d");

    auto a_v = AsArray(info[2]);
    bool ret{};

    auto v = AsS32(a_v.Get(0u));

    ret = ImGui::VSliderInt(GetStrUtf8(0), AsVec2(info[1]), &v, AsS32(info[3]), AsS32(info[4]),
        GetStrUtf8(1), AsS32Or(info[6], 0));

    a_v.Set(0u, FromS32(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value InputText(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto a_text = AsArray(info[1]);
    bool ret{};

    auto text = AsStrUtf8(a_text.Get(0u));

    ret = ImGui::InputText(GetStrUtf8(0), &text, AsS32Or(info[2], 0));

    a_text.Set(0u, FromStrUtf8(info.Env(), text));

    return FromBool(info.Env(), ret);
}

Napi::Value InputTextMultiline(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto a_text = AsArray(info[1]);
    bool ret{};

    auto text = AsStrUtf8(a_text.Get(0u));

    ret = ImGui::InputTextMultiline(GetStrUtf8(0), &text, AsVec2Or(info[2], {}),
        AsS32Or(info[3], 0));

    a_text.Set(0u, FromStrUtf8(info.Env(), text));

    return FromBool(info.Env(), ret);
}

Napi::Value InputTextWithHint(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8(1, info[1]);

    auto a_text = AsArray(info[2]);
    bool ret{};

    auto text = AsStrUtf8(a_text.Get(0u));

    ret = ImGui::InputTextWithHint(GetStrUtf8(0), GetStrUtf8(1), &text, AsS32Or(info[3], 0));

    a_text.Set(0u, FromStrUtf8(info.Env(), text));

    return FromBool(info.Env(), ret);
}

Napi::Value InputFloat(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[4], "%.3f");

    auto a_v = AsArray(info[1]);
    bool ret{};

    auto v = AsF32(a_v.Get(0u));

    ret = ImGui::InputFloat(GetStrUtf8(0), &v, AsF32Or(info[2], 0.0f), AsF32Or(info[3], 0.0f),
        GetStrUtf8(1), AsS32Or(info[5], 0));

    a_v.Set(0u, FromF32(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value InputFloats(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[4], "%.3f");

    auto v = AsArray(info[1]);
    auto step = AsF32Or(info[2], 0.0f);
    auto step_fast = AsF32Or(info[3], 0.0f);

    std::vector<float> floats(v.Length());
    bool ret{};

    for (std::size_t i{}; i < floats.size(); ++i)
        floats[i] = AsF32(v.Get(static_cast<u32>(i)));

    ret = ImGui::InputScalarN(GetStrUtf8(0), ImGuiDataType_Float, floats.data(), static_cast<int>(floats.size()),
        &step, &step_fast, GetStrUtf8(1), AsS32Or(info[5], 0));

    for (std::size_t i{}; i < floats.size(); ++i)
        v.Set(static_cast<u32>(i), FromF32(info.Env(), floats[i]));

    return FromBool(info.Env(), ret);
}

Napi::Value InputInt(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto a_v = AsArray(info[1]);
    bool ret{};

    auto v = AsS32(a_v.Get(0u));

    ret = ImGui::InputInt(GetStrUtf8(0), &v, AsS32Or(info[2], 1), AsS32Or(info[3], 100),
        AsS32Or(info[4], 0));

    a_v.Set(0u, FromS32(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value InputInts(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[4], "%d");

    auto v = AsArray(info[1]);
    auto step = AsS32Or(info[2], 1);
    auto step_fast = AsS32Or(info[3], 100);

    std::vector<int> ints(v.Length());
    bool ret{};

    for (std::size_t i{}; i < ints.size(); ++i)
        ints[i] = AsS32(v.Get(static_cast<u32>(i)));

    ret = ImGui::InputScalarN(GetStrUtf8(0), ImGuiDataType_S32, ints.data(), static_cast<int>(ints.size()),
        &step, &step_fast, GetStrUtf8(1), AsS32Or(info[5], 0));

    for (std::size_t i{}; i < ints.size(); ++i)
        v.Set(static_cast<u32>(i), FromS32(info.Env(), ints[i]));

    return FromBool(info.Env(), ret);
}

Napi::Value InputDouble(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[4], "%.6f");

    auto a_v = info[1].As<Napi::Object>();
    bool ret{};

    auto v = AsF64(a_v.Get(0u));

    ret = ImGui::InputDouble(GetStrUtf8(0), &v, AsF64Or(info[2], 0.0), AsF64Or(info[3], 0.0),
        GetStrUtf8(1), AsS32Or(info[5], 0));

    a_v.Set(0u, FromF64(info.Env(), v));

    return FromBool(info.Env(), ret);
}

Napi::Value ColorEdit3(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto o_col = AsObject(info[1]);
    bool ret{};

    auto col = AsColor(o_col);

    ret = ImGui::ColorEdit3(GetStrUtf8(0), &col.Value.x, AsS32Or(info[2], 0));

    FromColor(info.Env(), o_col, col);

    return FromBool(info.Env(), ret);
}

Napi::Value ColorEdit4(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto o_col = AsObject(info[1]);
    bool ret{};

    auto col = AsColor(o_col);

    ret = ImGui::ColorEdit4(GetStrUtf8(0), &col.Value.x, AsS32Or(info[2], 0));

    FromColor(info.Env(), o_col, col);

    return FromBool(info.Env(), ret);
}

Napi::Value ColorPicker3(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto o_col = AsObject(info[1]);
    bool ret{};

    auto col = AsColor(o_col);

    ret = ImGui::ColorPicker3(GetStrUtf8(0), &col.Value.x, AsS32Or(info[2], 0));

    FromColor(info.Env(), o_col, col);

    return FromBool(info.Env(), ret);
}

Napi::Value ColorPicker4(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto o_col = AsObject(info[1]);
    auto ref_col = AsColorOr(info[3], {});
    bool ret{};

    auto col = AsColor(o_col);

    ret = ImGui::ColorPicker4(GetStrUtf8(0), &col.Value.x, AsS32Or(info[2], 0),
        info[3].IsUndefined() ? nullptr : &ref_col.Value.x);

    FromColor(info.Env(), o_col, col);

    return FromBool(info.Env(), ret);
}

Napi::Value ColorButton(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::ColorButton(GetStrUtf8(0), AsColor(info[1]).Value,
        AsS32Or(info[2], 0), AsVec2Or(info[3], {})));
}

Napi::Value SetColorEditOptions(const Napi::CallbackInfo& info)
{
    ImGui::SetColorEditOptions(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value TreeNode(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::TreeNodeEx(GetStrUtf8(0), AsS32Or(info[1], 0)));
}

Napi::Value TreeNodeEx(const Napi::CallbackInfo& info)
{
    bool ret{};

    if (info[0].IsString()) {
        LoadStrUtf8(0, info[0]);
        LoadStrUtf8(1, info[1]);
        ret = ImGui::TreeNodeEx(GetStrUtf8(0), AsS32Or(info[2], 0), "%s", GetStrUtf8(1));
    }
    else if (info[0].IsNumber()) {
        LoadStrUtf8(0, info[1]);
        ret = ImGui::TreeNodeEx(AsPtr(info[0]), AsS32Or(info[2], 0), "%s", GetStrUtf8(0));
    }

    return FromBool(info.Env(), ret);
}

Napi::Value TreePush(const Napi::CallbackInfo& info)
{
    if (info.Length() == 0) {
        ImGui::TreePush();
    }
    else if (info[0].IsString()) {
        LoadStrUtf8(0, info[0]);
        ImGui::TreePush(GetStrUtf8(0));
    }
    else if (info[0].IsNumber()) {
        ImGui::TreePush(AsPtr(info[0]));
    }

    return info.Env().Undefined();
}

Napi::Value TreePop(const Napi::CallbackInfo& info)
{
    ImGui::TreePop();

    return info.Env().Undefined();
}

Napi::Value GetTreeNodeToLabelSpacing(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetTreeNodeToLabelSpacing());
}

Napi::Value CollapsingHeader(const Napi::CallbackInfo& info)
{
    const bool closable = info.Length() >= 2 && info[0].IsString() && info[1].IsObject();
    bool ret{};

    if (!closable) {
        LoadStrUtf8(0, info[0]);
        ret = ImGui::CollapsingHeader(GetStrUtf8(0), AsS32Or(info[1], 0));
    }
    else {
        auto a_open = AsArray(info[1]);

        auto open = AsBool(a_open.Get(0u));

        LoadStrUtf8(0, info[0]);
        ret = ImGui::CollapsingHeader(GetStrUtf8(0), &open, AsS32Or(info[2], 0));

        a_open.Set(0u, FromBool(info.Env(), ret));
    }

    return FromBool(info.Env(), ret);
}

Napi::Value SetNextItemOpen(const Napi::CallbackInfo& info)
{
    ImGui::SetNextItemOpen(AsBool(info[0]), AsS32Or(info[1], 0));

    return info.Env().Undefined();
}

Napi::Value Selectable(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    const bool by_ref = info.Length() >= 2 && info[1].IsObject();
    bool ret{};

    if (!by_ref) {
        ret = ImGui::Selectable(GetStrUtf8(0), AsBoolOr(info[1], false), AsS32Or(info[2], 0), AsVec2Or(info[3], {}));
    }
    else {
        auto a_selected = AsArray(info[1]);

        auto selected = AsBool(a_selected.Get(0u));

        ret = ImGui::Selectable(GetStrUtf8(0), &selected, AsS32Or(info[2], 0), AsVec2Or(info[3], {}));

        a_selected.Set(0u, FromBool(info.Env(), selected));
    }

    return FromBool(info.Env(), ret);
}

Napi::Value ListBox(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto a_current_item = AsArray(info[1]);
    auto items = AsArray(info[2]);
    bool ret{};

    auto current_item = AsS32(a_current_item.Get(0u));

    const auto func = [](void* data, int idx, const char** out_text) {
        static char str[512];
        auto real_data = static_cast<Napi::Array*>(data);
        std::strcpy(str, real_data->Get(idx).ToString().Utf8Value().c_str());
        *out_text = str;
        return static_cast<unsigned>(idx) < real_data->Length();
    };

    ret = ImGui::ListBox(GetStrUtf8(0), &current_item, func, &items, items.Length(),
        AsS32Or(info[3], -1));

    a_current_item.Set(0u, FromS32(info.Env(), current_item));

    return FromBool(info.Env(), ret);
}

Napi::Value ListBoxHeader(const Napi::CallbackInfo& info)
{
    const bool size_override = info.Length() >= 2 && (info[1].IsUndefined() || info[1].IsObject());
    bool ret{};

    LoadStrUtf8(0, info[0]);

    if (!size_override)
        ret = ImGui::ListBoxHeader(GetStrUtf8(0), AsS32(info[1]), AsS32Or(info[2], -1));
    else
        ret = ImGui::ListBoxHeader(GetStrUtf8(0), AsVec2Or(info[1], {}));

    return FromBool(info.Env(), ret);
}

Napi::Value ListBoxFooter(const Napi::CallbackInfo& info)
{
    ImGui::ListBoxFooter();

    return info.Env().Undefined();
}

Napi::Value PlotLines(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[3], "");

    auto values = AsArray(info[1]);

    const auto func = [](void* data, int idx) {
        auto real_data = static_cast<Napi::Array*>(data);
        return AsF32(real_data->Get(idx));
    };

    ImGui::PlotLines(GetStrUtf8(0), func, &values, values.Length(), AsS32Or(info[2], 0),
        GetStrUtf8(1)[0] == '\0' ? nullptr : GetStrUtf8(1), AsF32Or(info[4], FLT_MAX),
        AsF32Or(info[5], FLT_MAX), AsVec2Or(info[6], {}));

    return info.Env().Undefined();
}

Napi::Value PlotHistogram(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[3], "");

    auto values = AsArray(info[1]);

    const auto func = [](void* data, int idx) {
        auto real_data = static_cast<Napi::Array*>(data);
        return AsF32(real_data->Get(idx));
    };

    ImGui::PlotHistogram(GetStrUtf8(0), func, &values, values.Length(), AsS32Or(info[2], 0),
        GetStrUtf8(1)[0] == '\0' ? nullptr : GetStrUtf8(1), AsF32Or(info[4], FLT_MAX),
        AsF32Or(info[5], FLT_MAX), AsVec2Or(info[6], {}));

    return info.Env().Undefined();
}

Napi::Value ValueBool(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::Value(GetStrUtf8(0), AsBool(info[1]));

    return info.Env().Undefined();
}

Napi::Value ValueInt(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::Value(GetStrUtf8(0), AsS32(info[1]));

    return info.Env().Undefined();
}

Napi::Value ValueFloat(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[2], "");
    ImGui::Value(GetStrUtf8(0), AsF32(info[1]), GetStrUtf8(1)[0] == '\0' ? nullptr : GetStrUtf8(1));

    return info.Env().Undefined();
}

Napi::Value BeginMenuBar(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::BeginMenuBar());
}

Napi::Value EndMenuBar(const Napi::CallbackInfo& info)
{
    ImGui::EndMenuBar();

    return info.Env().Undefined();
}

Napi::Value BeginMainMenuBar(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::BeginMainMenuBar());
}

Napi::Value EndMainMenuBar(const Napi::CallbackInfo& info)
{
    ImGui::EndMainMenuBar();

    return info.Env().Undefined();
}

Napi::Value BeginMenu(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::BeginMenu(GetStrUtf8(0), AsBoolOr(info[1], true)));
}

Napi::Value EndMenu(const Napi::CallbackInfo& info)
{
    ImGui::EndMenu();

    return info.Env().Undefined();
}

Napi::Value MenuItem(const Napi::CallbackInfo& info)
{
    bool by_ref = info.Length() >= 3 && info[2].IsObject();
    bool ret{};

    LoadStrUtf8(0, info[0]);
    LoadStrUtf8Or(1, info[1], "");

    if (!by_ref) {
        ret = ImGui::MenuItem(GetStrUtf8(0), GetStrUtf8(1), AsBoolOr(info[2], false),
            AsBoolOr(info[3], true));
    }
    else {
        auto a_selected = AsArray(info[2]);

        auto selected = AsBool(a_selected.Get(0u));

        ret = ImGui::MenuItem(GetStrUtf8(0), GetStrUtf8(1), &selected, AsBoolOr(info[3], true));

        a_selected.Set(0u, FromBool(info.Env(), selected));
    }

    return FromBool(info.Env(), ret);
}

Napi::Value BeginTooltip(const Napi::CallbackInfo& info)
{
    ImGui::BeginTooltip();

    return info.Env().Undefined();
}

Napi::Value EndTooltip(const Napi::CallbackInfo& info)
{
    ImGui::EndTooltip();

    return info.Env().Undefined();
}

Napi::Value SetTooltip(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::SetTooltip("%s", GetStrUtf8(0));

    return info.Env().Undefined();
}

Napi::Value BeginPopup(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::BeginPopup(GetStrUtf8(0), AsS32Or(info[1], 0)));
}

Napi::Value BeginPopupModal(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto a_open = AsArray(info[1]);
    bool ret{};

    if (a_open.IsUndefined()) {
        ret = ImGui::BeginPopupModal(GetStrUtf8(0), nullptr, AsS32Or(info[2], 0));
    }
    else {
        auto open = AsBool(a_open.Get(0u));

        ret = ImGui::BeginPopupModal(GetStrUtf8(0), &open, AsS32Or(info[2], 0));

        a_open.Set(0u, FromBool(info.Env(), open));
    }

    return FromBool(info.Env(), ret);
}

Napi::Value EndPopup(const Napi::CallbackInfo& info)
{
    ImGui::EndPopup();

    return info.Env().Undefined();
}

Napi::Value OpenPopup(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::OpenPopup(GetStrUtf8(0), AsS32Or(info[1], 0));

    return info.Env().Undefined();
}

Napi::Value OpenPopupContextItem(const Napi::CallbackInfo& info)
{
    LoadStrUtf8Or(0, info[0], "");

    return FromBool(info.Env(), ImGui::OpenPopupContextItem(GetStrUtf8(0)[0] == '\0' ? nullptr : GetStrUtf8(0), AsS32Or(info[1], 1)));
}

Napi::Value CloseCurrentPopup(const Napi::CallbackInfo& info)
{
    ImGui::CloseCurrentPopup();

    return info.Env().Undefined();
}

Napi::Value BeginPopupContextItem(const Napi::CallbackInfo& info)
{
    LoadStrUtf8Or(0, info[0], "");

    return FromBool(info.Env(), ImGui::BeginPopupContextItem(
        GetStrUtf8(0)[0] == '\0' ? nullptr : GetStrUtf8(0), AsS32Or(info[1], 1)));
}

Napi::Value BeginPopupContextWindow(const Napi::CallbackInfo& info)
{
    LoadStrUtf8Or(0, info[0], "");

    return FromBool(info.Env(), ImGui::BeginPopupContextWindow(
        GetStrUtf8(0)[0] == '\0' ? nullptr : GetStrUtf8(0), AsS32Or(info[1], 1)));
}

Napi::Value BeginPopupContextVoid(const Napi::CallbackInfo& info)
{
    LoadStrUtf8Or(0, info[0], "");

    return FromBool(info.Env(), ImGui::BeginPopupContextVoid(
        GetStrUtf8(0)[0] == '\0' ? nullptr : GetStrUtf8(0), AsS32Or(info[1], 1)));
}

Napi::Value IsPopupOpen(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::IsPopupOpen(GetStrUtf8(0), AsS32Or(info[1], 0)));
}

Napi::Value Columns(const Napi::CallbackInfo& info)
{
    LoadStrUtf8Or(0, info[1], "");

    ImGui::Columns(AsS32Or(info[0], 1), GetStrUtf8(0)[0] == '\0' ? nullptr : GetStrUtf8(0),
        AsBoolOr(info[2], true));

    return info.Env().Undefined();
}

Napi::Value NextColumn(const Napi::CallbackInfo& info)
{
    ImGui::NextColumn();

    return info.Env().Undefined();
}

Napi::Value GetColumnIndex(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), ImGui::GetColumnIndex());
}

Napi::Value GetColumnWidth(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetColumnWidth(AsS32Or(info[0], -1)));
}

Napi::Value SetColumnWidth(const Napi::CallbackInfo& info)
{
    ImGui::SetColumnWidth(AsS32(info[0]), AsF32(info[1]));

    return info.Env().Undefined();
}

Napi::Value GetColumnOffset(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), ImGui::GetColumnOffset(AsS32Or(info[0], -1)));
}

Napi::Value SetColumnOffset(const Napi::CallbackInfo& info)
{
    ImGui::SetColumnOffset(AsS32(info[0]), AsF32(info[1]));

    return info.Env().Undefined();
}

Napi::Value GetColumnsCount(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), ImGui::GetColumnsCount());
}

Napi::Value BeginTabBar(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromBool(info.Env(), ImGui::BeginTabBar(GetStrUtf8(0), AsS32Or(info[1], 0)));
}

Napi::Value EndTabBar(const Napi::CallbackInfo& info)
{
    ImGui::EndTabBar();

    return info.Env().Undefined();
}

Napi::Value BeginTabItem(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    auto a_open = AsArray(info[1]);
    bool ret{};

    if (a_open.IsUndefined()) {
        ret = ImGui::BeginTabItem(GetStrUtf8(0), nullptr, AsS32Or(info[2], 0));
    }
    else {
        auto open = AsBool(a_open.Get(0u));

        ret = ImGui::BeginTabItem(GetStrUtf8(0), &open, AsS32Or(info[2], 0));

        a_open.Set(0u, FromBool(info.Env(), open));
    }

    return FromBool(info.Env(), ret);
}

Napi::Value EndTabItem(const Napi::CallbackInfo& info)
{
    ImGui::EndTabItem();

    return info.Env().Undefined();
}

Napi::Value SetTabItemClosed(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    ImGui::SetTabItemClosed(GetStrUtf8(0));

    return info.Env().Undefined();
}

Napi::Value DockSpace(const Napi::CallbackInfo& info)
{
    ImGui::DockSpace(AsU32(info[0]), AsVec2Or(info[1], {}), AsS32Or(info[2], 0));

    return info.Env().Undefined();
}

Napi::Value DockSpaceOverViewport(const Napi::CallbackInfo& info)
{
    ImGui::DockSpaceOverViewport(AsViewportPtrOr(info[0], {}), AsS32Or(info[1], 0));

    return info.Env().Undefined();
}

Napi::Value GetWindowDockId(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), ImGui::GetWindowDockID());
}

Napi::Value IsWindowDocked(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsWindowDocked());
}

Napi::Value LogToTTY(const Napi::CallbackInfo& info)
{
    ImGui::LogToTTY(AsS32Or(info[0], -1));

    return info.Env().Undefined();
}

Napi::Value LogToFile(const Napi::CallbackInfo& info)
{
    ImGui::LogToFile(AsS32Or(info[0], -1), info[1].IsUndefined() ? nullptr : AsPath(info[1]).string().c_str());

    return info.Env().Undefined();
}

Napi::Value LogToClipboard(const Napi::CallbackInfo& info)
{
    ImGui::LogToClipboard(AsS32Or(info[0], -1));

    return info.Env().Undefined();
}

Napi::Value LogFinish(const Napi::CallbackInfo& info)
{
    ImGui::LogFinish();

    return info.Env().Undefined();
}

Napi::Value LogButtons(const Napi::CallbackInfo& info)
{
    ImGui::LogButtons();

    return info.Env().Undefined();
}

Napi::Value LogText(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);
    ImGui::LogText("%s", GetStrUtf8(0));

    return info.Env().Undefined();
}

Napi::Value PushClipRect(const Napi::CallbackInfo& info)
{
    ImGui::PushClipRect(AsVec2(info[0]), AsVec2(info[1]), AsBool(info[2]));

    return info.Env().Undefined();
}

Napi::Value PopClipRect(const Napi::CallbackInfo& info)
{
    ImGui::PopClipRect();

    return info.Env().Undefined();
}

Napi::Value SetItemDefaultFocus(const Napi::CallbackInfo& info)
{
    ImGui::SetItemDefaultFocus();

    return info.Env().Undefined();
}

Napi::Value SetKeyboardFocusHere(const Napi::CallbackInfo& info)
{
    ImGui::SetKeyboardFocusHere(AsS32Or(info[0], 0));

    return info.Env().Undefined();
}

Napi::Value IsItemHovered(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemHovered(AsS32Or(info[0], 0)));
}

Napi::Value IsItemActive(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemActive());
}

Napi::Value IsItemFocused(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemFocused());
}

Napi::Value IsItemClicked(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemClicked(AsS32Or(info[0], 0)));
}

Napi::Value IsItemVisible(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemVisible());
}

Napi::Value IsItemEdited(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemEdited());
}

Napi::Value IsItemActivated(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemActivated());
}

Napi::Value IsItemDeactivated(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemDeactivated());
}

Napi::Value IsItemDeactivatedAfterEdit(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemDeactivatedAfterEdit());
}

Napi::Value IsItemToggledOpen(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsItemToggledOpen());
}

Napi::Value IsAnyItemHovered(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsAnyItemHovered());
}

Napi::Value IsAnyItemActive(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsAnyItemActive());
}

Napi::Value IsAnyItemFocused(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsAnyItemFocused());
}

Napi::Value GetItemRectMin(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetItemRectMin());
}

Napi::Value GetItemRectMax(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetItemRectMax());
}

Napi::Value GetItemRectSize(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetItemRectSize());
}

Napi::Value SetItemAllowOverlap(const Napi::CallbackInfo& info)
{
    ImGui::SetItemAllowOverlap();

    return info.Env().Undefined();
}

Napi::Value IsRectVisible(const Napi::CallbackInfo& info)
{
    bool ret{};

    if (info.Length() == 1)
        ret = ImGui::IsRectVisible(AsVec2(info[0]));
    else if (info.Length() == 2)
        ret = ImGui::IsRectVisible(AsVec2(info[0]), AsVec2(info[1]));

    return FromBool(info.Env(), ret);
}

Napi::Value GetTime(const Napi::CallbackInfo& info)
{
    return FromF64(info.Env(), ImGui::GetTime());
}

Napi::Value GetFrameCount(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), ImGui::GetFrameCount());
}

Napi::Value GetBackgroundDrawList(const Napi::CallbackInfo& info)
{
    if (info.Length() == 0) {
        return DrawListRef::Create(info.Env(), ImGui::GetBackgroundDrawList(),
            ImGui::GetWindowViewport()->GetWorkPos());
    }
    else if (info.Length() == 1) {
        auto viewport = AsViewportPtr(info[0]);

        return DrawListRef::Create(info.Env(), ImGui::GetBackgroundDrawList(viewport),
            viewport->GetWorkPos());
    }

    return info.Env().Undefined();
}

Napi::Value GetForegroundDrawList(const Napi::CallbackInfo& info)
{
    if (info.Length() == 0) {
        return DrawListRef::Create(info.Env(), ImGui::GetForegroundDrawList(),
            ImGui::GetWindowViewport()->GetWorkPos());
    }
    else if (info.Length() == 1) {
        auto viewport = AsViewportPtr(info[0]);

        return DrawListRef::Create(info.Env(), ImGui::GetForegroundDrawList(viewport),
            viewport->GetWorkPos());
    }

    return info.Env().Undefined();
}

Napi::Value GetStyleColorName(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), ImGui::GetStyleColorName(AsS32(info[0])));
}

Napi::Value CalcListClipping(const Napi::CallbackInfo& info)
{
    int out_items_display_start, out_items_display_end;
    ImGui::CalcListClipping(AsS32(info[0]), AsF32(info[1]), &out_items_display_start,
        &out_items_display_end);

    return FromVec2(info.Env(), {static_cast<float>(out_items_display_start),
        static_cast<float>(out_items_display_end)});
}

Napi::Value BeginChildFrame(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::BeginChildFrame(AsU32(info[0]), AsVec2(info[1]),
        AsS32Or(info[2], 0)));
}

Napi::Value EndChildFrame(const Napi::CallbackInfo& info)
{
    ImGui::EndChildFrame();

    return info.Env().Undefined();
}

Napi::Value CalcTextSize(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromVec2(info.Env(), ImGui::CalcTextSize(GetStrUtf8(0), nullptr,
        AsBoolOr(info[1], false), AsF32Or(info[2], -1.0f)));
}

Napi::Value ColorConvertRGBtoHSV(const Napi::CallbackInfo& info)
{
    auto from_color_ = AsColor(info[0]);
    ImColor to_color;

    ImGui::ColorConvertRGBtoHSV(from_color_.Value.x, from_color_.Value.y, from_color_.Value.z,
        to_color.Value.x, to_color.Value.y, to_color.Value.z);

    return FromColor(info.Env(), to_color);
}

Napi::Value ColorConvertHSVtoRGB(const Napi::CallbackInfo& info)
{
    auto from_color_ = AsColor(info[0]);
    ImColor to_color;

    ImGui::ColorConvertHSVtoRGB(from_color_.Value.x, from_color_.Value.y, from_color_.Value.z,
        to_color.Value.x, to_color.Value.y, to_color.Value.z);

    return FromColor(info.Env(), to_color);
}

Napi::Value IsKeyDown(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsKeyDown(AsS32(info[0])));
}

Napi::Value IsKeyPressed(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsKeyPressed(AsS32(info[0]),
        AsBoolOr(info[1], true)));
}

Napi::Value IsKeyReleased(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsKeyReleased(AsS32(info[0])));
}

Napi::Value GetKeyPressedAmount(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), ImGui::GetKeyPressedAmount(AsS32(info[0]),
        AsF32(info[1]), AsF32(info[2])));
}

Napi::Value CaptureKeyboardFromApp(const Napi::CallbackInfo& info)
{
    ImGui::CaptureKeyboardFromApp(AsBoolOr(info[0], true));

    return info.Env().Undefined();
}

Napi::Value IsMouseDown(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsMouseDown(AsS32(info[0])));
}

Napi::Value IsMouseClicked(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsMouseClicked(AsS32(info[0]),
        AsBoolOr(info[1], false)));
}

Napi::Value IsMouseReleased(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsMouseReleased(AsS32(info[0])));
}

Napi::Value IsMouseDoubleClicked(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsMouseDoubleClicked(AsS32(info[0])));
}

Napi::Value IsMouseHoveringRect(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsMouseHoveringRect(AsVec2(info[0]),
        AsVec2(info[1]), AsBoolOr(info[2], true)));
}

Napi::Value IsMousePosValid(const Napi::CallbackInfo& info)
{
    bool ret{};

    if (info[0].IsUndefined()) {
        ret = ImGui::IsMousePosValid();
    }
    else {
        auto mouse_pos = AsVec2(info[0]);

        ret = ImGui::IsMousePosValid(&mouse_pos);
    }

    return FromBool(info.Env(), ret);
}

Napi::Value IsAnyMouseDown(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsAnyMouseDown());
}

Napi::Value GetMousePos(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetMousePos());
}

Napi::Value GetMousePosOnOpeningCurrentPopup(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetMousePosOnOpeningCurrentPopup());
}

Napi::Value IsMouseDragging(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), ImGui::IsMouseDragging(AsS32(info[0]),
        AsF32Or(info[1], -1.0f)));
}

Napi::Value GetMouseDragDelta(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), ImGui::GetMouseDragDelta(AsS32Or(info[0], 0),
        AsF32Or(info[1], -1.0f)));
}

Napi::Value ResetMouseDragDelta(const Napi::CallbackInfo& info)
{
    ImGui::ResetMouseDragDelta(AsS32Or(info[0], 0));

    return info.Env().Undefined();
}

Napi::Value GetMouseCursor(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), ImGui::GetMouseCursor());
}

Napi::Value SetMouseCursor(const Napi::CallbackInfo& info)
{
    ImGui::SetMouseCursor(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value CaptureMouseFromApp(const Napi::CallbackInfo& info)
{
    ImGui::CaptureMouseFromApp(AsBoolOr(info[0], true));

    return info.Env().Undefined();
}

Napi::Value GetClipboardText(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), ImGui::GetClipboardText());
}

Napi::Value SetClipboardText(const Napi::CallbackInfo& info)
{
    ImGui::SetClipboardText(AsStrUtf8(info[0]).c_str());

    return info.Env().Undefined();
}

Napi::Value LoadIniSettingsFromDisk(const Napi::CallbackInfo& info)
{
    ImGui::LoadIniSettingsFromDisk(AsPath(info[0]).string().c_str());

    return info.Env().Undefined();
}

Napi::Value LoadIniSettingsFromMemory(const Napi::CallbackInfo& info)
{
    auto ini_data = info[0].As<Napi::Uint8Array>();

    ImGui::LoadIniSettingsFromMemory(reinterpret_cast<const char*>(ini_data.Data()),
        ini_data.ByteLength());

    return info.Env().Undefined();
}

Napi::Value SaveIniSettingsToDisk(const Napi::CallbackInfo& info)
{
    ImGui::SaveIniSettingsToDisk(AsPath(info[0]).string().c_str());

    return info.Env().Undefined();
}

Napi::Value SaveIniSettingsToMemory(const Napi::CallbackInfo& info)
{
    std::size_t out_ini_size;
    auto data = ImGui::SaveIniSettingsToMemory(&out_ini_size);

    return Napi::Buffer<std::uint8_t>::Copy(info.Env(), reinterpret_cast<const std::uint8_t*>(data),
        out_ini_size);
}

Napi::Value GetMainViewport(const Napi::CallbackInfo& info)
{
    return ViewportRef::Create(info.Env(), ImGui::GetMainViewport());
}

Napi::Value FindViewportById(const Napi::CallbackInfo& info)
{
    return ViewportRef::Create(info.Env(), ImGui::FindViewportByID(AsU32(info[0])));
}

Napi::Value PushItemDisabled(const Napi::CallbackInfo& info)
{
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, AsBool(info[0]));

    return info.Env().Undefined();
}

Napi::Value PopItemDisabled(const Napi::CallbackInfo& info)
{
    ImGui::PopItemFlag();

    return info.Env().Undefined();
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    DrawListRef::Init(env, exports);
    FontAtlasRef::Init(env, exports);
    FontRef::Init(env, exports);
    IORef::Init(env, exports);
    StyleRef::Init(env, exports);
    ViewportRef::Init(env, exports);

    exports.Set("getIo", Napi::Function::New(env, GetIO));
    exports.Set("getStyle", Napi::Function::New(env, GetStyle));

    exports.Set("showDemoWindow", Napi::Function::New(env, ShowDemoWindow));
    exports.Set("showAboutWindow", Napi::Function::New(env, ShowAboutWindow));
    exports.Set("showMetricsWindow", Napi::Function::New(env, ShowMetricsWindow));
    exports.Set("showStyleEditor", Napi::Function::New(env, ShowStyleEditor));
    exports.Set("showStyleSelector", Napi::Function::New(env, ShowStyleSelector));
    exports.Set("showFontSelector", Napi::Function::New(env, ShowFontSelector));
    exports.Set("showUserGuide", Napi::Function::New(env, ShowUserGuide));
    exports.Set("getVersion", Napi::Function::New(env, GetVersion));

    exports.Set("styleColorsDark", Napi::Function::New(env, StyleColorsDark));
    exports.Set("styleColorsClassic", Napi::Function::New(env, StyleColorsClassic));
    exports.Set("styleColorsLight", Napi::Function::New(env, StyleColorsLight));

    exports.Set("begin", Napi::Function::New(env, Begin));
    exports.Set("end", Napi::Function::New(env, End));

    exports.Set("beginChild", Napi::Function::New(env, BeginChild));
    exports.Set("endChild", Napi::Function::New(env, EndChild));

    exports.Set("isWindowAppearing", Napi::Function::New(env, IsWindowAppearing));
    exports.Set("isWindowCollapsed", Napi::Function::New(env, IsWindowCollapsed));
    exports.Set("isWindowFocused", Napi::Function::New(env, IsWindowFocused));
    exports.Set("isWindowHovered", Napi::Function::New(env, IsWindowHovered));
    exports.Set("getWindowDrawList", Napi::Function::New(env, GetWindowDrawList));
    exports.Set("getWindowDpiScale", Napi::Function::New(env, GetWindowDPIScale));
    exports.Set("getWindowViewport", Napi::Function::New(env, GetWindowViewport));
    exports.Set("getWindowPos", Napi::Function::New(env, GetWindowPos));
    exports.Set("getWindowSize", Napi::Function::New(env, GetWindowSize));
    exports.Set("getWindowWidth", Napi::Function::New(env, GetWindowWidth));
    exports.Set("getWindowHeight", Napi::Function::New(env, GetWindowHeight));

    exports.Set("setNextWindowPos", Napi::Function::New(env, SetNextWindowPos));
    exports.Set("setNextWindowSize", Napi::Function::New(env, SetNextWindowSize));
    exports.Set("setNextWindowSizeConstraints", Napi::Function::New(env, SetNextWindowSizeConstraints));
    exports.Set("setNextWindowContentSize", Napi::Function::New(env, SetNextWindowContentSize));
    exports.Set("setNextWindowCollapsed", Napi::Function::New(env, SetNextWindowCollapsed));
    exports.Set("setNextWindowFocus", Napi::Function::New(env, SetNextWindowFocus));
    exports.Set("setNextWindowBgAlpha", Napi::Function::New(env, SetNextWindowBgAlpha));
    exports.Set("setNextWindowViewport", Napi::Function::New(env, SetNextWindowViewport));
    exports.Set("setWindowPos", Napi::Function::New(env, SetWindowPos));
    exports.Set("setWindowSize", Napi::Function::New(env, SetWindowSize));
    exports.Set("setWindowCollapsed", Napi::Function::New(env, SetWindowCollapsed));
    exports.Set("setWindowFocus", Napi::Function::New(env, SetWindowFocus));
    exports.Set("setWindowFontScale", Napi::Function::New(env, SetWindowFontScale));

    exports.Set("getContentRegionMax", Napi::Function::New(env, GetContentRegionMax));
    exports.Set("getContentRegionAvail", Napi::Function::New(env, GetContentRegionAvail));
    exports.Set("getWindowContentRegionMin", Napi::Function::New(env, GetWindowContentRegionMin));
    exports.Set("getWindowContentRegionMax", Napi::Function::New(env, GetWindowContentRegionMax));
    exports.Set("getWindowContentRegionWidth", Napi::Function::New(env, GetWindowContentRegionWidth));

    exports.Set("getScrollX", Napi::Function::New(env, GetScrollX));
    exports.Set("getScrollY", Napi::Function::New(env, GetScrollY));
    exports.Set("getScrollMaxX", Napi::Function::New(env, GetScrollMaxX));
    exports.Set("getScrollMaxY", Napi::Function::New(env, GetScrollMaxY));
    exports.Set("setScrollX", Napi::Function::New(env, SetScrollX));
    exports.Set("setScrollY", Napi::Function::New(env, SetScrollY));
    exports.Set("setScrollHereX", Napi::Function::New(env, SetScrollHereX));
    exports.Set("setScrollHereY", Napi::Function::New(env, SetScrollHereY));
    exports.Set("setScrollFromPosX", Napi::Function::New(env, SetScrollFromPosX));
    exports.Set("setScrollFromPosY", Napi::Function::New(env, SetScrollFromPosY));

    exports.Set("pushFont", Napi::Function::New(env, PushFont));
    exports.Set("popFont", Napi::Function::New(env, PopFont));
    exports.Set("pushStyleColor", Napi::Function::New(env, PushStyleColor));
    exports.Set("popStyleColor", Napi::Function::New(env, PopStyleColor));
    exports.Set("pushStyleVar", Napi::Function::New(env, PushStyleVar));
    exports.Set("popStyleVar", Napi::Function::New(env, PopStyleVar));
    exports.Set("getStyleColor", Napi::Function::New(env, GetStyleColor));
    exports.Set("getFont", Napi::Function::New(env, GetFont));
    exports.Set("getFontSize", Napi::Function::New(env, GetFontSize));
    exports.Set("getFontTexUvWhitePixel", Napi::Function::New(env, GetFontTexUvWhitePixel));
    exports.Set("getColor", Napi::Function::New(env, GetColor));

    exports.Set("pushItemWidth", Napi::Function::New(env, PushItemWidth));
    exports.Set("popItemWidth", Napi::Function::New(env, PopItemWidth));
    exports.Set("setNextItemWidth", Napi::Function::New(env, SetNextItemWidth));
    exports.Set("calcItemWidth", Napi::Function::New(env, CalcItemWidth));
    exports.Set("pushTextWrapPos", Napi::Function::New(env, PushTextWrapPos));
    exports.Set("popTextWrapPos", Napi::Function::New(env, PopTextWrapPos));
    exports.Set("pushAllowKeyboardFocus", Napi::Function::New(env, PushAllowKeyboardFocus));
    exports.Set("popAllowKeyboardFocus", Napi::Function::New(env, PopAllowKeyboardFocus));
    exports.Set("pushButtonRepeat", Napi::Function::New(env, PushButtonRepeat));
    exports.Set("popButtonRepeat", Napi::Function::New(env, PopButtonRepeat));

    exports.Set("separator", Napi::Function::New(env, Separator));
    exports.Set("sameLine", Napi::Function::New(env, SameLine));
    exports.Set("newLine", Napi::Function::New(env, NewLine));
    exports.Set("spacing", Napi::Function::New(env, Spacing));
    exports.Set("dummy", Napi::Function::New(env, Dummy));
    exports.Set("indent", Napi::Function::New(env, Indent));
    exports.Set("unindent", Napi::Function::New(env, Unindent));
    exports.Set("beginGroup", Napi::Function::New(env, BeginGroup));
    exports.Set("endGroup", Napi::Function::New(env, EndGroup));
    exports.Set("getCursorPos", Napi::Function::New(env, GetCursorPos));
    exports.Set("getCursorPosX", Napi::Function::New(env, GetCursorPosX));
    exports.Set("getCursorPosY", Napi::Function::New(env, GetCursorPosY));
    exports.Set("setCursorPos", Napi::Function::New(env, SetCursorPos));
    exports.Set("setCursorPosX", Napi::Function::New(env, SetCursorPosX));
    exports.Set("setCursorPosY", Napi::Function::New(env, SetCursorPosY));
    exports.Set("getCursorStartPos", Napi::Function::New(env, GetCursorStartPos));
    exports.Set("getCursorScreenPos", Napi::Function::New(env, GetCursorScreenPos));
    exports.Set("setCursorScreenPos", Napi::Function::New(env, SetCursorScreenPos));
    exports.Set("alignTextToFramePadding", Napi::Function::New(env, AlignTextToFramePadding));
    exports.Set("getTextLineHeight", Napi::Function::New(env, GetTextLineHeight));
    exports.Set("getTextLineHeightWithSpacing", Napi::Function::New(env, GetTextLineHeightWithSpacing));
    exports.Set("getFrameHeight", Napi::Function::New(env, GetFrameHeight));
    exports.Set("getFrameHeightWithSpacing", Napi::Function::New(env, GetFrameHeightWithSpacing));

    exports.Set("pushId", Napi::Function::New(env, PushId));
    exports.Set("popId", Napi::Function::New(env, PopId));
    exports.Set("getId", Napi::Function::New(env, GetId));

    exports.Set("text", Napi::Function::New(env, Text));
    exports.Set("textColored", Napi::Function::New(env, TextColored));
    exports.Set("textDisabled", Napi::Function::New(env, TextDisabled));
    exports.Set("textWrapped", Napi::Function::New(env, TextWrapped));
    exports.Set("labelText", Napi::Function::New(env, LabelText));
    exports.Set("bulletText", Napi::Function::New(env, BulletText));

    exports.Set("button", Napi::Function::New(env, Button));
    exports.Set("smallButton", Napi::Function::New(env, SmallButton));
    exports.Set("invisibleButton", Napi::Function::New(env, InvisibleButton));
    exports.Set("arrowButton", Napi::Function::New(env, ArrowButton));
    exports.Set("image", Napi::Function::New(env, Image));
    exports.Set("imageButton", Napi::Function::New(env, ImageButton));
    exports.Set("checkbox", Napi::Function::New(env, Checkbox));
    exports.Set("checkboxFlags", Napi::Function::New(env, CheckboxFlags));
    exports.Set("radioButton", Napi::Function::New(env, RadioButton));
    exports.Set("progressBar", Napi::Function::New(env, ProgressBar));
    exports.Set("bullet", Napi::Function::New(env, BulletText));

    exports.Set("beginCombo", Napi::Function::New(env, BeginCombo));
    exports.Set("endCombo", Napi::Function::New(env, EndCombo));
    exports.Set("combo", Napi::Function::New(env, Combo));

    exports.Set("dragFloat", Napi::Function::New(env, DragFloat));
    exports.Set("dragFloats", Napi::Function::New(env, DragFloats));
    exports.Set("dragFloatRange", Napi::Function::New(env, DragFloatRange));
    exports.Set("dragInt", Napi::Function::New(env, DragInt));
    exports.Set("dragInts", Napi::Function::New(env, DragInts));
    exports.Set("dragIntRange", Napi::Function::New(env, DragIntRange));

    exports.Set("sliderFloat", Napi::Function::New(env, SliderFloat));
    exports.Set("sliderFloats", Napi::Function::New(env, SliderFloats));
    exports.Set("sliderAngle", Napi::Function::New(env, SliderAngle));
    exports.Set("sliderInt", Napi::Function::New(env, SliderInt));
    exports.Set("sliderInts", Napi::Function::New(env, SliderInts));
    exports.Set("vSliderFloat", Napi::Function::New(env, VSliderFloat));
    exports.Set("vSliderInt", Napi::Function::New(env, VSliderInt));

    exports.Set("inputText", Napi::Function::New(env, InputText));
    exports.Set("inputTextMultiline", Napi::Function::New(env, InputTextMultiline));
    exports.Set("inputTextWithHint", Napi::Function::New(env, InputTextWithHint));
    exports.Set("inputFloat", Napi::Function::New(env, InputFloat));
    exports.Set("inputFloats", Napi::Function::New(env, InputFloats));
    exports.Set("inputInt", Napi::Function::New(env, InputInt));
    exports.Set("inputInts", Napi::Function::New(env, InputInts));
    exports.Set("inputDouble", Napi::Function::New(env, InputDouble));

    exports.Set("colorEdit3", Napi::Function::New(env, ColorEdit3));
    exports.Set("colorEdit4", Napi::Function::New(env, ColorEdit4));
    exports.Set("colorPicker3", Napi::Function::New(env, ColorPicker3));
    exports.Set("colorPicker4", Napi::Function::New(env, ColorPicker4));
    exports.Set("colorButton", Napi::Function::New(env, ColorButton));
    exports.Set("setColorEditOptions", Napi::Function::New(env, SetColorEditOptions));

    exports.Set("treeNode", Napi::Function::New(env, TreeNode));
    exports.Set("treeNodeEx", Napi::Function::New(env, TreeNodeEx));
    exports.Set("treePush", Napi::Function::New(env, TreePush));
    exports.Set("treePop", Napi::Function::New(env, TreePop));
    exports.Set("getTreeNodeToLabelSpacing", Napi::Function::New(env, GetTreeNodeToLabelSpacing));
    exports.Set("collapsingHeader", Napi::Function::New(env, CollapsingHeader));
    exports.Set("setNextItemOpen", Napi::Function::New(env, SetNextItemOpen));

    exports.Set("selectable", Napi::Function::New(env, Selectable));

    exports.Set("listBox", Napi::Function::New(env, ListBox));
    exports.Set("listBoxHeader", Napi::Function::New(env, ListBoxHeader));
    exports.Set("listBoxFooter", Napi::Function::New(env, ListBoxFooter));

    exports.Set("plotLines", Napi::Function::New(env, PlotLines));
    exports.Set("plotHistogram", Napi::Function::New(env, PlotHistogram));

    exports.Set("valueBool", Napi::Function::New(env, ValueBool));
    exports.Set("valueInt", Napi::Function::New(env, ValueInt));
    exports.Set("valueFloat", Napi::Function::New(env, ValueFloat));

    exports.Set("beginMenuBar", Napi::Function::New(env, BeginMenuBar));
    exports.Set("endMenuBar", Napi::Function::New(env, EndMenuBar));
    exports.Set("beginMainMenuBar", Napi::Function::New(env, BeginMainMenuBar));
    exports.Set("endMainMenuBar", Napi::Function::New(env, EndMainMenuBar));
    exports.Set("beginMenu", Napi::Function::New(env, BeginMenu));
    exports.Set("endMenu", Napi::Function::New(env, EndMenu));
    exports.Set("menuItem", Napi::Function::New(env, MenuItem));

    exports.Set("beginTooltip", Napi::Function::New(env, BeginTooltip));
    exports.Set("endTooltip", Napi::Function::New(env, EndTooltip));
    exports.Set("setTooltip", Napi::Function::New(env, SetTooltip));

    exports.Set("beginPopup", Napi::Function::New(env, BeginPopup));
    exports.Set("beginPopupModal", Napi::Function::New(env, BeginPopupModal));
    exports.Set("endPopup", Napi::Function::New(env, EndPopup));

    exports.Set("openPopup", Napi::Function::New(env, OpenPopup));
    exports.Set("openPopupContextItem", Napi::Function::New(env, OpenPopupContextItem));
    exports.Set("closeCurrentPopup", Napi::Function::New(env, CloseCurrentPopup));

    exports.Set("beginPopupContextItem", Napi::Function::New(env, BeginPopupContextItem));
    exports.Set("beginPopupContextWindow", Napi::Function::New(env, BeginPopupContextWindow));
    exports.Set("beginPopupContextVoid", Napi::Function::New(env, BeginPopupContextVoid));

    exports.Set("isPopupOpen", Napi::Function::New(env, IsPopupOpen));

    exports.Set("columns", Napi::Function::New(env, Columns));
    exports.Set("nextColumn", Napi::Function::New(env, NextColumn));
    exports.Set("getColumnIndex", Napi::Function::New(env, GetColumnIndex));
    exports.Set("getColumnWidth", Napi::Function::New(env, GetColumnWidth));
    exports.Set("setColumnWidth", Napi::Function::New(env, SetColumnWidth));
    exports.Set("getColumnOffset", Napi::Function::New(env, GetColumnOffset));
    exports.Set("setColumnOffset", Napi::Function::New(env, SetColumnOffset));
    exports.Set("getColumnsCount", Napi::Function::New(env, GetColumnsCount));

    exports.Set("beginTabBar", Napi::Function::New(env, BeginTabBar));
    exports.Set("endTabBar", Napi::Function::New(env, EndTabBar));
    exports.Set("beginTabItem", Napi::Function::New(env, BeginTabItem));
    exports.Set("endTabItem", Napi::Function::New(env, EndTabItem));
    exports.Set("setTabItemClosed", Napi::Function::New(env, SetTabItemClosed));

    exports.Set("dockSpace", Napi::Function::New(env, DockSpace));
    exports.Set("dockSpaceOverViewport", Napi::Function::New(env, DockSpaceOverViewport));
    exports.Set("getWindowDockId", Napi::Function::New(env, GetWindowDockId));
    exports.Set("isWindowDocked", Napi::Function::New(env, IsWindowDocked));

    exports.Set("logToTTY", Napi::Function::New(env, LogToTTY));
    exports.Set("logToFile", Napi::Function::New(env, LogToFile));
    exports.Set("logToClipboard", Napi::Function::New(env, LogToClipboard));
    exports.Set("logFinish", Napi::Function::New(env, LogFinish));
    exports.Set("logButtons", Napi::Function::New(env, LogButtons));
    exports.Set("logText", Napi::Function::New(env, LogText));

    exports.Set("pushClipRect", Napi::Function::New(env, PushClipRect));
    exports.Set("popClipRect", Napi::Function::New(env, PopClipRect));

    exports.Set("setItemDefaultFocus", Napi::Function::New(env, SetItemDefaultFocus));
    exports.Set("setKeyboardFocusHere", Napi::Function::New(env, SetKeyboardFocusHere));

    exports.Set("isItemHovered", Napi::Function::New(env, IsItemHovered));
    exports.Set("isItemActive", Napi::Function::New(env, IsItemActive));
    exports.Set("isItemFocused", Napi::Function::New(env, IsItemFocused));
    exports.Set("isItemClicked", Napi::Function::New(env, IsItemClicked));
    exports.Set("isItemVisible", Napi::Function::New(env, IsItemVisible));
    exports.Set("isItemEdited", Napi::Function::New(env, IsItemEdited));
    exports.Set("isItemActivated", Napi::Function::New(env, IsItemActivated));
    exports.Set("isItemDeactivated", Napi::Function::New(env, IsItemDeactivated));
    exports.Set("isItemDeactivatedAfterEdit", Napi::Function::New(env, IsItemDeactivatedAfterEdit));
    exports.Set("isItemToggledOpen", Napi::Function::New(env, IsItemToggledOpen));
    exports.Set("isAnyItemHovered", Napi::Function::New(env, IsAnyItemHovered));
    exports.Set("isAnyItemActive", Napi::Function::New(env, IsAnyItemActive));
    exports.Set("isAnyItemFocused", Napi::Function::New(env, IsAnyItemFocused));
    exports.Set("getItemRectMin", Napi::Function::New(env, GetItemRectMin));
    exports.Set("getItemRectMax", Napi::Function::New(env, GetItemRectMax));
    exports.Set("getItemRectSize", Napi::Function::New(env, GetItemRectSize));
    exports.Set("setItemAllowOverlap", Napi::Function::New(env, SetItemAllowOverlap));

    exports.Set("isRectVisible", Napi::Function::New(env, IsRectVisible));
    exports.Set("getTime", Napi::Function::New(env, GetTime));
    exports.Set("getFrameCount", Napi::Function::New(env, GetFrameCount));
    exports.Set("getBackgroundDrawList", Napi::Function::New(env, GetBackgroundDrawList));
    exports.Set("getForegroundDrawList", Napi::Function::New(env, GetForegroundDrawList));
    exports.Set("getStyleColorName", Napi::Function::New(env, GetStyleColorName));
    exports.Set("calcListClipping", Napi::Function::New(env, CalcListClipping));
    exports.Set("beginChildFrame", Napi::Function::New(env, BeginChildFrame));
    exports.Set("endChildFrame", Napi::Function::New(env, EndChildFrame));

    exports.Set("calcTextSize", Napi::Function::New(env, CalcTextSize));

    exports.Set("colorConvertRGBtoHSV", Napi::Function::New(env, ColorConvertRGBtoHSV));
    exports.Set("colorConvertHSVtoRGB", Napi::Function::New(env, ColorConvertHSVtoRGB));

    exports.Set("isKeyDown", Napi::Function::New(env, IsKeyDown));
    exports.Set("isKeyPressed", Napi::Function::New(env, IsKeyPressed));
    exports.Set("isKeyReleased", Napi::Function::New(env, IsKeyReleased));
    exports.Set("getKeyPressedAmount", Napi::Function::New(env, GetKeyPressedAmount));
    exports.Set("captureKeyboardFromApp", Napi::Function::New(env, CaptureKeyboardFromApp));

    exports.Set("isMouseDown", Napi::Function::New(env, IsMouseDown));
    exports.Set("isMouseClicked", Napi::Function::New(env, IsMouseClicked));
    exports.Set("isMouseReleased", Napi::Function::New(env, IsMouseReleased));
    exports.Set("isMouseDoubleClicked", Napi::Function::New(env, IsMouseDoubleClicked));
    exports.Set("isMouseHoveringRect", Napi::Function::New(env, IsMouseHoveringRect));
    exports.Set("isMousePosValid", Napi::Function::New(env, IsMousePosValid));
    exports.Set("isAnyMouseDown", Napi::Function::New(env, IsAnyMouseDown));
    exports.Set("getMousePos", Napi::Function::New(env, GetMousePos));
    exports.Set("getMousePosOnOpeningCurrentPopup", Napi::Function::New(env, GetMousePosOnOpeningCurrentPopup));
    exports.Set("isMouseDragging", Napi::Function::New(env, IsMouseDragging));
    exports.Set("getMouseDragDelta", Napi::Function::New(env, GetMouseDragDelta));
    exports.Set("resetMouseDragDelta", Napi::Function::New(env, ResetMouseDragDelta));
    exports.Set("getMouseCursor", Napi::Function::New(env, GetMouseCursor));
    exports.Set("setMouseCursor", Napi::Function::New(env, SetMouseCursor));
    exports.Set("captureMouseFromApp", Napi::Function::New(env, CaptureMouseFromApp));

    exports.Set("getClipboardText", Napi::Function::New(env, GetClipboardText));
    exports.Set("setClipboardText", Napi::Function::New(env, SetClipboardText));

    exports.Set("loadIniSettingsFromDisk", Napi::Function::New(env, LoadIniSettingsFromDisk));
    exports.Set("loadIniSettingsFromMemory", Napi::Function::New(env, LoadIniSettingsFromMemory));
    exports.Set("saveIniSettingsToDisk", Napi::Function::New(env, SaveIniSettingsToDisk));
    exports.Set("saveIniSettingsToMemory", Napi::Function::New(env, SaveIniSettingsToMemory));

    exports.Set("getMainViewport", Napi::Function::New(env, GetMainViewport));
    exports.Set("findViewportById", Napi::Function::New(env, FindViewportById));

    exports.Set("pushItemDisabled", Napi::Function::New(env, PushItemDisabled));
    exports.Set("popItemDisabled", Napi::Function::New(env, PopItemDisabled));

    return exports;
}

}
