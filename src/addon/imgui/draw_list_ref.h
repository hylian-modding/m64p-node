#pragma once

#include "imgui/imgui.h"

#include <napi.h>

namespace Addon::ImGui_ {

struct DrawListRef : Napi::ObjectWrap<DrawListRef> {
    static Napi::FunctionReference constructor;
	ImDrawList* m_draw_list;
	ImVec2 m_origin;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
	static Napi::Object Create(Napi::Env env, ImDrawList* draw_list, const ImVec2& origin);

    DrawListRef(const Napi::CallbackInfo& info);

	Napi::Value GetFlags(const Napi::CallbackInfo& info);
	void SetFlags(const Napi::CallbackInfo& info, const Napi::Value& value);

	Napi::Value PushClipRect(const Napi::CallbackInfo& info);
	Napi::Value PushClipRectFullScreen(const Napi::CallbackInfo& info);
	Napi::Value PopClipRect(const Napi::CallbackInfo& info);
	Napi::Value PushTextureId(const Napi::CallbackInfo& info);
	Napi::Value	PopTextureId(const Napi::CallbackInfo& info);
	Napi::Value GetClipRectMin(const Napi::CallbackInfo& info);
	Napi::Value GetClipRectMax(const Napi::CallbackInfo& info);

	Napi::Value AddLine(const Napi::CallbackInfo& info);
	Napi::Value AddRect(const Napi::CallbackInfo& info);
	Napi::Value AddRectFilled(const Napi::CallbackInfo& info);
	Napi::Value AddRectFilledMultiColor(const Napi::CallbackInfo& info);
	Napi::Value AddQuad(const Napi::CallbackInfo& info);
	Napi::Value AddQuadFilled(const Napi::CallbackInfo& info);
	Napi::Value AddTriangle(const Napi::CallbackInfo& info);
	Napi::Value AddTriangleFilled(const Napi::CallbackInfo& info);
	Napi::Value AddCircle(const Napi::CallbackInfo& info);
	Napi::Value AddCircleFilled(const Napi::CallbackInfo& info);
	Napi::Value AddNgon(const Napi::CallbackInfo& info);
	Napi::Value AddNgonFilled(const Napi::CallbackInfo& info);
	Napi::Value AddText(const Napi::CallbackInfo& info);
    Napi::Value AddTextEx(const Napi::CallbackInfo& info);
	Napi::Value AddPolyline(const Napi::CallbackInfo& info);
	Napi::Value AddConvexPolyFilled(const Napi::CallbackInfo& info);
	Napi::Value AddBezierCurve(const Napi::CallbackInfo& info);

	Napi::Value AddImage(const Napi::CallbackInfo& info);
	Napi::Value AddImageQuad(const Napi::CallbackInfo& info);
	Napi::Value AddImageRounded(const Napi::CallbackInfo& info);

	Napi::Value PathClear(const Napi::CallbackInfo& info);
	Napi::Value PathLineTo(const Napi::CallbackInfo& info);
	Napi::Value PathLineToMergeDuplicate(const Napi::CallbackInfo& info);
	Napi::Value PathFillConvex(const Napi::CallbackInfo& info);
	Napi::Value PathStroke(const Napi::CallbackInfo& info);
	Napi::Value PathArcTo(const Napi::CallbackInfo& info);
	Napi::Value PathArcToFast(const Napi::CallbackInfo& info);
	Napi::Value PathBezierCurveTo(const Napi::CallbackInfo& info);
	Napi::Value PathRect(const Napi::CallbackInfo& info);

	Napi::Value AddDrawCmd(const Napi::CallbackInfo& info);
};

}
