#pragma once

#include "common/types.h"
#include "imgui/imgui.h"

#include <napi.h>

namespace Addon::ImGui_::Param {

ImVec2 AsVec2(const Napi::Value& value);
ImVec2 AsVec2Or(const Napi::Value& value, const ImVec2& default_value);
std::vector<ImVec2> AsVec2Array(const Napi::Value& value);
Napi::Value FromVec2(Napi::Env env, const ImVec2& value);
Napi::Value FromVec2(Napi::Env env, int x, int y);
ImVec4 AsVec4(const Napi::Value& value);
Napi::Value FromVec4(Napi::Env env, const ImVec4& value);
Napi::Value FromVec4(Napi::Env env, int x, int y, int z, int w);
ImColor AsColor(const Napi::Value& value);
ImColor AsColorOr(const Napi::Value& value, const ImColor& default_value);
Napi::Value FromColor(Napi::Env env, const ImColor& value);
void FromColor(Napi::Env env, Napi::Object& obj, const ImColor& value);
u32 AsU32Color(const Napi::Value& value);
u32 AsU32ColorOr(const Napi::Value& value, u32 default_value);
ImFont* AsFontPtr(const Napi::Value& value);
ImFont* AsFontPtrOr(const Napi::Value& value, ImFont* default_value);
ImGuiViewport* AsViewportPtr(const Napi::Value& value);
ImGuiViewport* AsViewportPtrOr(const Napi::Value& value, ImGuiViewport* default_value);
ImDrawList* AsDrawListPtr(const Napi::Value& value);

}
