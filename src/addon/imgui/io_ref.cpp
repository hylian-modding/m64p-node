#include "addon/imgui/param.h"
#include "addon/imgui/font_atlas_ref.h"
#include "addon/imgui/font_ref.h"
#include "addon/imgui/io_ref.h"
#include "addon/param.h"

using namespace Param;
using namespace Addon::ImGui_::Param;

namespace Addon::ImGui_ {

static std::string s_ini_filename;
static std::string s_log_filename;

Napi::FunctionReference IORef::constructor;

Napi::Object IORef::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "IoRef", {
		InstanceAccessor<&IORef::GetConfigFlags, &IORef::SetConfigFlags>("configFlags"),
		InstanceAccessor<&IORef::GetBackendFlags, &IORef::SetBackendFlags>("backendFlags"),
		InstanceAccessor<&IORef::GetDisplaySize, &IORef::SetDisplaySize>("displaySize"),
		InstanceAccessor<&IORef::GetDeltaTime, &IORef::SetDeltaTime>("deltaTime"),
		InstanceAccessor<&IORef::GetIniSavingRate, &IORef::SetIniSavingRate>("iniSavingRate"),
		InstanceAccessor<&IORef::GetIniFilename, &IORef::SetIniFilename>("iniFilename"),
		InstanceAccessor<&IORef::GetLogFilename, &IORef::SetLogFilename>("logFilename"),
		InstanceAccessor<&IORef::GetMouseDoubleClickTime, &IORef::SetMouseDoubleClickTime>("mouseDoubleClickTime"),
		InstanceAccessor<&IORef::GetMouseDoubleClickMaxDist, &IORef::SetMouseDoubleClickMaxDist>("mouseDoubleClickMaxDist"),
		InstanceAccessor<&IORef::GetMouseDragThreshold, &IORef::SetMouseDragThreshold>("mouseDragThreshold"),
		InstanceAccessor<&IORef::GetKeyRepeatDelay, &IORef::SetKeyRepeatDelay>("keyRepeatDelay"),
		InstanceAccessor<&IORef::GetKeyRepeatRate, &IORef::SetKeyRepeatRate>("keyRepeatRate"),

		InstanceAccessor<&IORef::GetFonts>("fonts"),
		InstanceAccessor<&IORef::GetFontGlobalScale, &IORef::SetFontGlobalScale>("fontGlobalScale"),
		InstanceAccessor<&IORef::GetFontAllowUserScaling, &IORef::SetFontAllowUserScaling>("fontAllowUserScaling"),
		InstanceAccessor<&IORef::GetFontDefault, &IORef::SetFontDefault>("fontDefault"),
		InstanceAccessor<&IORef::GetDisplayFramebufferScale, &IORef::SetDisplayFramebufferScale>("displayFramebufferScale"),

		InstanceAccessor<&IORef::GetConfigDockingNoSplit, &IORef::SetConfigDockingNoSplit>("configDockingNoSplit"),
		InstanceAccessor<&IORef::GetConfigDockingWithShift, &IORef::SetConfigDockingWithShift>("configDockingWithShift"),
		InstanceAccessor<&IORef::GetConfigDockingAlwaysTabBar, &IORef::SetConfigDockingAlwaysTabBar>("configDockingAlwaysTabBar"),
		InstanceAccessor<&IORef::GetConfigDockingTransparentPayload, &IORef::SetConfigDockingTransparentPayload>("configDockingTransparentPayload"),

		InstanceAccessor<&IORef::GetConfigViewportsNoAutoMerge, &IORef::SetConfigViewportsNoAutoMerge>("configViewportsNoAutoMerge"),
		InstanceAccessor<&IORef::GetConfigViewportsNoTaskBarIcon, &IORef::SetConfigViewportsNoTaskBarIcon>("configViewportsNoTaskBarIcon"),
		InstanceAccessor<&IORef::GetConfigViewportsNoDecoration, &IORef::SetConfigViewportsNoDecoration>("configViewportsNoDecoration"),
		InstanceAccessor<&IORef::GetConfigViewportsNoDefaultParent, &IORef::SetConfigViewportsNoDefaultParent>("configViewportsNoDefaultParent"),

		InstanceAccessor<&IORef::GetMouseDrawCursor, &IORef::SetMouseDrawCursor>("configMouseDrawCursor"),
		InstanceAccessor<&IORef::GetConfigInputTextCursorBlink, &IORef::SetConfigInputTextCursorBlink>("configInputTextCursorBlink"),
		InstanceAccessor<&IORef::GetConfigWindowsResizeFromEdges, &IORef::SetConfigWindowsResizeFromEdges>("configWindowsResizeFromEdges"),
		InstanceAccessor<&IORef::GetConfigWindowsMoveFromTitleBarOnly, &IORef::SetConfigWindowsMoveFromTitleBarOnly>("configWindowsMoveFromTitleBarOnly"),
		InstanceAccessor<&IORef::GetConfigWindowsMemoryCompactTimer, &IORef::SetConfigWindowsMemoryCompactTimer>("configWindowsMemoryCompactTimer"),

		InstanceAccessor<&IORef::GetBackendPlatformName>("backendPlatformName"),
		InstanceAccessor<&IORef::GetBackendRendererName>("backendRendererName"),

		InstanceAccessor<&IORef::GetWantCaptureMouse>("wantCaptureMouse"),
		InstanceAccessor<&IORef::GetWantCaptureKeyboard>("wantCaptureKeyboard"),
		InstanceAccessor<&IORef::GetWantTextInput>("wantTextInput"),
		InstanceAccessor<&IORef::GetWantSetMousePos>("wantSetMousePos"),
		InstanceAccessor<&IORef::GetWantSaveIniSettings>("wantSaveIniSettings"),
		InstanceAccessor<&IORef::GetNavActive>("navActive"),
		InstanceAccessor<&IORef::GetNavVisible>("navVisible"),
		InstanceAccessor<&IORef::GetFramerate>("framerate"),
		InstanceAccessor<&IORef::GetMetricsRenderVertices>("metricsRenderVertices"),
		InstanceAccessor<&IORef::GetMetricsRenderIndices>("metricsRenderIndices"),
		InstanceAccessor<&IORef::GetMetricsRenderWindows>("metricsRenderWindows"),
		InstanceAccessor<&IORef::GetMetricsActiveWindows>("metricsActiveWindows"),
		InstanceAccessor<&IORef::GetMetricsActiveAllocations>("metricsActiveAllocations"),
		InstanceAccessor<&IORef::GetMouseDelta>("mouseDelta")
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("IoRef", func);

    return exports;
}

Napi::Object IORef::Create(Napi::Env env, ImGuiIO* io)
{
    return constructor.New({FromPtr(env, io)});
}

IORef::IORef(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<IORef>{info}
{
    m_io = static_cast<ImGuiIO*>(AsPtr(info[0]));
}

Napi::Value IORef::GetConfigFlags(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_io->ConfigFlags);
}

void IORef::SetConfigFlags(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigFlags = AsS32(value);
}

Napi::Value IORef::GetBackendFlags(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_io->BackendFlags);
}

void IORef::SetBackendFlags(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->BackendFlags = AsS32(value);
}

Napi::Value IORef::GetDisplaySize(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_io->DisplaySize);
}

void IORef::SetDisplaySize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->DisplaySize = AsVec2(value);
}

Napi::Value IORef::GetDeltaTime(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->DeltaTime);
}

void IORef::SetDeltaTime(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->DeltaTime = AsF32(value);
}

Napi::Value IORef::GetIniSavingRate(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->IniSavingRate);
}

void IORef::SetIniSavingRate(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->IniSavingRate = AsF32(value);
}

Napi::Value IORef::GetIniFilename(const Napi::CallbackInfo& info)
{
	return !m_io->IniFilename ? info.Env().Undefined() : FromStrUtf8(info.Env(), m_io->IniFilename);
}

void IORef::SetIniFilename(const Napi::CallbackInfo&, const Napi::Value& value)
{
	if (value.IsUndefined()) {
		m_io->IniFilename = nullptr;
    }
	else {
		s_ini_filename = AsPath(value).string();
		m_io->IniFilename = s_ini_filename.c_str();
	}
}

Napi::Value IORef::GetLogFilename(const Napi::CallbackInfo& info)
{
	return !m_io->LogFilename ? info.Env().Undefined() : FromStrUtf8(info.Env(), m_io->LogFilename);
}

void IORef::SetLogFilename(const Napi::CallbackInfo&, const Napi::Value& value)
{
	if (value.IsUndefined()) {
		m_io->LogFilename = nullptr;
    }
	else {
		s_log_filename = AsPath(value).string();
		m_io->LogFilename = s_log_filename.c_str();
	}
}

Napi::Value IORef::GetMouseDoubleClickTime(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->MouseDoubleClickTime);
}

void IORef::SetMouseDoubleClickTime(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->MouseDoubleClickTime = AsF32(value);
}

Napi::Value IORef::GetMouseDoubleClickMaxDist(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->MouseDoubleClickMaxDist);
}

void IORef::SetMouseDoubleClickMaxDist(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->MouseDoubleClickMaxDist = AsF32(value);
}

Napi::Value IORef::GetMouseDragThreshold(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->MouseDragThreshold);
}

void IORef::SetMouseDragThreshold(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->MouseDragThreshold = AsF32(value);
}

Napi::Value IORef::GetKeyRepeatDelay(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->KeyRepeatDelay);
}

void IORef::SetKeyRepeatDelay(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->KeyRepeatDelay = AsF32(value);
}

Napi::Value IORef::GetKeyRepeatRate(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->KeyRepeatRate);
}

void IORef::SetKeyRepeatRate(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->KeyRepeatRate = AsF32(value);
}

Napi::Value IORef::GetFonts(const Napi::CallbackInfo& info)
{
	return FontAtlasRef::Create(info.Env(), m_io->Fonts);
}

Napi::Value IORef::GetFontGlobalScale(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->FontGlobalScale);
}

void IORef::SetFontGlobalScale(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->FontGlobalScale = AsF32(value);
}

Napi::Value IORef::GetFontAllowUserScaling(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->FontAllowUserScaling);
}

void IORef::SetFontAllowUserScaling(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->FontAllowUserScaling = AsBool(value);
}

Napi::Value IORef::GetFontDefault(const Napi::CallbackInfo& info)
{
	return !m_io->FontDefault ? info.Env().Undefined() : FontRef::Create(info.Env(), m_io->FontDefault);
}

void IORef::SetFontDefault(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->FontDefault = value.IsUndefined() ? nullptr : AsFontPtr(value);
}

Napi::Value IORef::GetDisplayFramebufferScale(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_io->DisplayFramebufferScale);
}

void IORef::SetDisplayFramebufferScale(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->DisplayFramebufferScale = AsVec2(value);
}

Napi::Value IORef::GetConfigDockingNoSplit(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigDockingNoSplit);
}

void IORef::SetConfigDockingNoSplit(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigDockingNoSplit = AsBool(value);
}

Napi::Value IORef::GetConfigDockingWithShift(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigDockingWithShift);
}

void IORef::SetConfigDockingWithShift(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigDockingWithShift = AsBool(value);
}

Napi::Value IORef::GetConfigDockingAlwaysTabBar(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigDockingAlwaysTabBar);
}

void IORef::SetConfigDockingAlwaysTabBar(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigDockingAlwaysTabBar = AsBool(value);
}

Napi::Value IORef::GetConfigDockingTransparentPayload(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigDockingTransparentPayload);
}

void IORef::SetConfigDockingTransparentPayload(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigDockingTransparentPayload = AsBool(value);
}

Napi::Value IORef::GetConfigViewportsNoAutoMerge(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigViewportsNoAutoMerge);
}

void IORef::SetConfigViewportsNoAutoMerge(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigViewportsNoAutoMerge = AsBool(value);
}

Napi::Value IORef::GetConfigViewportsNoTaskBarIcon(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigViewportsNoTaskBarIcon);
}

void IORef::SetConfigViewportsNoTaskBarIcon(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigViewportsNoTaskBarIcon = AsBool(value);
}

Napi::Value IORef::GetConfigViewportsNoDecoration(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigViewportsNoDecoration);
}

void IORef::SetConfigViewportsNoDecoration(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigViewportsNoDecoration = AsBool(value);
}

Napi::Value IORef::GetConfigViewportsNoDefaultParent(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigViewportsNoDefaultParent);
}

void IORef::SetConfigViewportsNoDefaultParent(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigViewportsNoDefaultParent = AsBool(value);
}

Napi::Value IORef::GetMouseDrawCursor(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->MouseDrawCursor);
}

void IORef::SetMouseDrawCursor(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->MouseDrawCursor = AsBool(value);
}

Napi::Value IORef::GetConfigInputTextCursorBlink(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigInputTextCursorBlink);
}

void IORef::SetConfigInputTextCursorBlink(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigInputTextCursorBlink = AsBool(value);
}

Napi::Value IORef::GetConfigWindowsResizeFromEdges(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigWindowsResizeFromEdges);
}

void IORef::SetConfigWindowsResizeFromEdges(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigWindowsResizeFromEdges = AsBool(value);
}

Napi::Value IORef::GetConfigWindowsMoveFromTitleBarOnly(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->ConfigWindowsMoveFromTitleBarOnly);
}

void IORef::SetConfigWindowsMoveFromTitleBarOnly(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigWindowsMoveFromTitleBarOnly = AsBool(value);
}

Napi::Value IORef::GetConfigWindowsMemoryCompactTimer(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->ConfigWindowsMemoryCompactTimer);
}

void IORef::SetConfigWindowsMemoryCompactTimer(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_io->ConfigWindowsMemoryCompactTimer = AsF32(value);
}

Napi::Value IORef::GetBackendPlatformName(const Napi::CallbackInfo& info)
{
	return !m_io->BackendPlatformName ? info.Env().Undefined() :
        FromStrUtf8(info.Env(), m_io->BackendPlatformName);
}

Napi::Value IORef::GetBackendRendererName(const Napi::CallbackInfo& info)
{
	return !m_io->BackendRendererName ? info.Env().Undefined() :
        FromStrUtf8(info.Env(), m_io->BackendRendererName);
}

Napi::Value IORef::GetWantCaptureMouse(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->WantCaptureMouse);
}

Napi::Value IORef::GetWantCaptureKeyboard(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->WantCaptureKeyboard);
}

Napi::Value IORef::GetWantTextInput(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->WantTextInput);
}

Napi::Value IORef::GetWantSetMousePos(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->WantSetMousePos);
}

Napi::Value IORef::GetWantSaveIniSettings(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->WantSaveIniSettings);
}

Napi::Value IORef::GetNavActive(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->NavActive);
}

Napi::Value IORef::GetNavVisible(const Napi::CallbackInfo& info)
{
	return FromBool(info.Env(), m_io->NavVisible);
}

Napi::Value IORef::GetFramerate(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_io->Framerate);
}

Napi::Value IORef::GetMetricsRenderVertices(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_io->MetricsRenderVertices);
}

Napi::Value IORef::GetMetricsRenderIndices(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_io->MetricsRenderIndices);
}

Napi::Value IORef::GetMetricsRenderWindows(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_io->MetricsRenderWindows);
}

Napi::Value IORef::GetMetricsActiveWindows(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_io->MetricsActiveWindows);
}

Napi::Value IORef::GetMetricsActiveAllocations(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_io->MetricsActiveAllocations);
}

Napi::Value IORef::GetMouseDelta(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_io->MouseDelta);
}

}
