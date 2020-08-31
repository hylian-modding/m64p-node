#include "addon/imgui/param.h"
#include "addon/imgui/draw_list_ref.h"
#include "addon/imgui/font_ref.h"
#include "addon/param.h"
#include "common/imvec2_ops.h"

using namespace Param;
using namespace Addon::ImGui_::Param;

namespace Addon::ImGui_ {

Napi::FunctionReference DrawListRef::constructor;

Napi::Object DrawListRef::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "DrawListRef", {
		InstanceAccessor<&DrawListRef::GetFlags, &DrawListRef::SetFlags>("flags"),

		InstanceMethod("pushClipRect", &DrawListRef::PushClipRect),
		InstanceMethod("pushClipRectFullScreen", &DrawListRef::PushClipRectFullScreen),
		InstanceMethod("popClipRect", &DrawListRef::PopClipRect),
		InstanceMethod("pushTextureId", &DrawListRef::PushTextureId),
		InstanceMethod("popTextureId", &DrawListRef::PopTextureId),
		InstanceMethod("getClipRectMin", &DrawListRef::GetClipRectMin),
		InstanceMethod("getClipRectMax", &DrawListRef::GetClipRectMax),

		InstanceMethod("addLine", &DrawListRef::AddLine),
		InstanceMethod("addRect", &DrawListRef::AddRect),
		InstanceMethod("addRectFilled", &DrawListRef::AddRectFilled),
		InstanceMethod("addRectFilledMultiColor", &DrawListRef::AddRectFilledMultiColor),
		InstanceMethod("addQuad", &DrawListRef::AddQuad),
		InstanceMethod("addQuadFilled", &DrawListRef::AddQuadFilled),
		InstanceMethod("addTriangle", &DrawListRef::AddTriangle),
		InstanceMethod("addTriangleFilled", &DrawListRef::AddTriangleFilled),
		InstanceMethod("addCircle", &DrawListRef::AddCircle),
		InstanceMethod("addCircleFilled", &DrawListRef::AddCircleFilled),
		InstanceMethod("addNgon", &DrawListRef::AddNgon),
		InstanceMethod("addNgonFilled", &DrawListRef::AddNgonFilled),
		InstanceMethod("addText", &DrawListRef::AddText),
        InstanceMethod("addTextEx", &DrawListRef::AddTextEx),
		InstanceMethod("addPolyline", &DrawListRef::AddPolyline),
		InstanceMethod("addConvexPolyFilled", &DrawListRef::AddConvexPolyFilled),
		InstanceMethod("addBezierCurve", &DrawListRef::AddBezierCurve),

		InstanceMethod("addImage", &DrawListRef::AddImage),
		InstanceMethod("addImageQuad", &DrawListRef::AddImageQuad),
		InstanceMethod("addImageRounded", &DrawListRef::AddImageRounded),

		InstanceMethod("pathClear", &DrawListRef::PathClear),
		InstanceMethod("pathLineTo", &DrawListRef::PathLineTo),
		InstanceMethod("pathLineToMergeDuplicate", &DrawListRef::PathLineToMergeDuplicate),
		InstanceMethod("pathFillConvex", &DrawListRef::PathFillConvex),
		InstanceMethod("pathStroke", &DrawListRef::PathStroke),
		InstanceMethod("pathArcTo", &DrawListRef::PathArcTo),
		InstanceMethod("pathArcToFast", &DrawListRef::PathArcToFast),
		InstanceMethod("pathBezierCurveTo", &DrawListRef::PathBezierCurveTo),
		InstanceMethod("pathRect", &DrawListRef::PathRect),

		InstanceMethod("addDrawCmd", &DrawListRef::AddDrawCmd)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("DrawListRef", func);

    return exports;
}

Napi::Object DrawListRef::Create(Napi::Env env, ImDrawList* draw_list, const ImVec2& origin)
{
    return constructor.New({
        FromPtr(env, draw_list),
		FromF32(env, origin.x),
		FromF32(env, origin.y)
    });
}

DrawListRef::DrawListRef(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<DrawListRef>{info}
{
	m_draw_list = static_cast<ImDrawList*>(AsPtr(info[0]));
	m_origin = {AsF32(info[1]), AsF32(info[2])};
}

Napi::Value DrawListRef::GetFlags(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_draw_list->Flags);
}

void DrawListRef::SetFlags(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_draw_list->Flags = AsS32(value);
}

Napi::Value DrawListRef::PushClipRect(const Napi::CallbackInfo& info)
{
	m_draw_list->PushClipRect(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]),
        AsBoolOr(info[2], false));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PushClipRectFullScreen(const Napi::CallbackInfo& info)
{
	m_draw_list->PushClipRectFullScreen();

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PopClipRect(const Napi::CallbackInfo& info)
{
	m_draw_list->PopClipRect();

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PushTextureId(const Napi::CallbackInfo& info)
{
	m_draw_list->PushTextureID(static_cast<ImTextureID>(AsPtr(info[0])));

	return info.Env().Undefined();
}

Napi::Value	DrawListRef::PopTextureId(const Napi::CallbackInfo& info)
{
	m_draw_list->PopTextureID();

	return info.Env().Undefined();
}

Napi::Value DrawListRef::GetClipRectMin(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_draw_list->GetClipRectMin());
}

Napi::Value DrawListRef::GetClipRectMax(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_draw_list->GetClipRectMax());
}

Napi::Value DrawListRef::AddLine(const Napi::CallbackInfo& info)
{
	m_draw_list->AddLine(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]), AsU32Color(info[2]),
        AsF32Or(info[3], 1.0f));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddRect(const Napi::CallbackInfo& info)
{
	m_draw_list->AddRect(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]), AsU32Color(info[2]),
        AsF32Or(info[3], 0.0f), AsS32Or(info[4], ImDrawCornerFlags_All), AsF32Or(info[5], 1.0f));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddRectFilled(const Napi::CallbackInfo& info)
{
	m_draw_list->AddRectFilled(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]), AsU32Color(info[2]),
        AsF32Or(info[3], 0.0f), AsS32Or(info[4], ImDrawCornerFlags_All));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddRectFilledMultiColor(const Napi::CallbackInfo& info)
{
	m_draw_list->AddRectFilledMultiColor(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]),
        AsU32Color(info[2]), AsU32Color(info[3]), AsU32Color(info[4]), AsU32Color(info[5]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddQuad(const Napi::CallbackInfo& info)
{
	m_draw_list->AddQuad(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]), m_origin + AsVec2(info[2]),
        m_origin + AsVec2(info[3]), AsU32Color(info[4]), AsF32Or(info[5], 1.0f));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddQuadFilled(const Napi::CallbackInfo& info)
{
	m_draw_list->AddQuadFilled(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]),
        m_origin + AsVec2(info[2]), m_origin + AsVec2(info[3]), AsU32Color(info[4]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddTriangle(const Napi::CallbackInfo& info)
{
	m_draw_list->AddTriangle(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]),
        m_origin + AsVec2(info[2]), AsU32Color(info[3]), AsF32Or(info[4], 1.0f));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddTriangleFilled(const Napi::CallbackInfo& info)
{
	m_draw_list->AddTriangleFilled(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]),
        m_origin + AsVec2(info[2]), AsU32Color(info[3]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddCircle(const Napi::CallbackInfo& info)
{
	m_draw_list->AddCircle(m_origin + AsVec2(info[0]), AsF32(info[1]), AsU32Color(info[2]),
        AsS32Or(info[3], 0), AsF32Or(info[4], 1.0f));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddCircleFilled(const Napi::CallbackInfo& info)
{
	m_draw_list->AddCircleFilled(m_origin + AsVec2(info[0]), AsF32(info[1]), AsU32Color(info[2]),
        AsS32Or(info[3], 0));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddNgon(const Napi::CallbackInfo& info)
{
	m_draw_list->AddNgon(m_origin + AsVec2(info[0]),  AsF32(info[1]), AsU32Color(info[2]),
        AsS32(info[3]), AsF32Or(info[4], 1.0f));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddNgonFilled(const Napi::CallbackInfo& info)
{
	m_draw_list->AddNgonFilled(m_origin + AsVec2(info[0]), AsF32(info[1]),
        AsU32Color(info[2]), AsS32(info[3]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddText(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[2]);
	m_draw_list->AddText(m_origin + AsVec2(info[0]), AsU32Color(info[1]), GetStrUtf8(0));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddTextEx(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[4]);
	m_draw_list->AddText(AsFontPtr(info[0]), AsF32(info[1]), m_origin + AsVec2(info[2]),
        AsU32Color(info[3]), GetStrUtf8(0), 0, AsF32Or(info[5], 0.0f));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddPolyline(const Napi::CallbackInfo& info)
{
	auto points = AsVec2Array(info[0]);

	std::transform(points.begin(), points.end(), points.begin(), [this](const ImVec2& point) {
		return m_origin + point;
	});

	m_draw_list->AddPolyline(points.data(), static_cast<int>(points.size()), AsU32Color(info[1]), AsBool(info[2]),
        AsF32(info[3]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddConvexPolyFilled(const Napi::CallbackInfo& info)
{
	auto points = AsVec2Array(info[0]);

	std::transform(points.begin(), points.end(), points.begin(), [this](const ImVec2& point) {
		return m_origin + point;
	});

	m_draw_list->AddConvexPolyFilled(points.data(), static_cast<int>(points.size()), AsU32Color(info[1]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddBezierCurve(const Napi::CallbackInfo& info)
{
	m_draw_list->AddBezierCurve(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]),
        m_origin + AsVec2(info[2]), m_origin + AsVec2(info[3]), AsU32Color(info[4]),
        AsF32(info[5]), AsS32Or(info[6], 0));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddImage(const Napi::CallbackInfo& info)
{
	m_draw_list->AddImage(static_cast<ImTextureID>(AsPtr(info[0])), m_origin + AsVec2(info[1]),
        m_origin + AsVec2(info[2]), AsVec2Or(info[3], {}), AsVec2Or(info[4], {1.0f, 1.0f}),
        AsU32ColorOr(info[5], IM_COL32_WHITE));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddImageQuad(const Napi::CallbackInfo& info)
{
	m_draw_list->AddImageQuad(static_cast<ImTextureID>(AsPtr(info[0])),
        m_origin + AsVec2(info[1]), m_origin + AsVec2(info[2]), m_origin + AsVec2(info[3]),
        m_origin + AsVec2(info[4]), AsVec2Or(info[5], {0.0f, 0.0f}),
        AsVec2Or(info[6], {1.0f, 0.0f}), AsVec2Or(info[7], {1.0f, 1.0f}),
        AsVec2Or(info[8], {0.0f, 1.0f}), AsU32ColorOr(info[9], IM_COL32_WHITE));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddImageRounded(const Napi::CallbackInfo& info)
{
	m_draw_list->AddImageRounded(static_cast<ImTextureID>(AsPtr(info[0])),
        m_origin + AsVec2(info[1]), m_origin + AsVec2(info[2]), AsVec2(info[3]), AsVec2(info[4]),
        AsU32Color(info[5]), AsF32(info[6]), AsS32Or(info[7], ImDrawCornerFlags_All));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PathClear(const Napi::CallbackInfo& info)
{
	m_draw_list->PathClear();

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PathLineTo(const Napi::CallbackInfo& info)
{
	m_draw_list->PathLineTo(m_origin + AsVec2(info[0]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PathLineToMergeDuplicate(const Napi::CallbackInfo& info)
{
	m_draw_list->PathLineToMergeDuplicate(m_origin + AsVec2(info[0]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PathFillConvex(const Napi::CallbackInfo& info)
{
	m_draw_list->PathFillConvex(AsU32Color(info[0]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PathStroke(const Napi::CallbackInfo& info)
{
	m_draw_list->PathStroke(AsU32Color(info[0]), AsBool(info[1]), AsF32Or(info[2], 1.0f));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PathArcTo(const Napi::CallbackInfo& info)
{
	m_draw_list->PathArcTo(m_origin + AsVec2(info[0]), AsF32(info[1]), AsF32(info[2]),
        AsF32(info[3]), AsS32Or(info[4], 10));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PathArcToFast(const Napi::CallbackInfo& info)
{
	m_draw_list->PathArcToFast(m_origin + AsVec2(info[0]), AsF32(info[1]),
        AsS32(info[2]), AsS32(info[3]));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PathBezierCurveTo(const Napi::CallbackInfo& info)
{
	m_draw_list->PathBezierCurveTo(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]),
        m_origin + AsVec2(info[2]), AsS32Or(info[3], 0));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::PathRect(const Napi::CallbackInfo& info)
{
	m_draw_list->PathRect(m_origin + AsVec2(info[0]), m_origin + AsVec2(info[1]),
        AsF32Or(info[2], 0.0f), AsS32Or(info[3], ImDrawCornerFlags_All));

	return info.Env().Undefined();
}

Napi::Value DrawListRef::AddDrawCmd(const Napi::CallbackInfo& info)
{
	m_draw_list->AddDrawCmd();

	return info.Env().Undefined();
}

}
