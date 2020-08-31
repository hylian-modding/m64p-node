#include "addon/imgui/param.h"
#include "addon/imgui/viewport_ref.h"
#include "addon/sdl/window_ref.h"
#include "addon/param.h"

using namespace Param;
using namespace Addon::ImGui_::Param;

namespace Addon::ImGui_ {

Napi::FunctionReference ViewportRef::constructor;

Napi::Object ViewportRef::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "ViewportRef", {
		InstanceAccessor<&ViewportRef::GetId>("id"),
		InstanceAccessor<&ViewportRef::GetFlags, &ViewportRef::SetFlags>("flags"),
		InstanceAccessor<&ViewportRef::GetPos, &ViewportRef::SetPos>("pos"),
		InstanceAccessor<&ViewportRef::GetSize, &ViewportRef::SetSize>("size"),
		InstanceAccessor<&ViewportRef::GetWorkOffsetMin, &ViewportRef::SetWorkOffsetMin>("workOffsetMin"),
		InstanceAccessor<&ViewportRef::GetWorkOffsetMax, &ViewportRef::SetWorkOffsetMax>("workOffsetMax"),
		InstanceAccessor<&ViewportRef::GetDPIScale, &ViewportRef::SetDPIScale>("dpiScale"),
		InstanceAccessor<&ViewportRef::GetParentViewportId>("parentViewportId"),
		InstanceAccessor<&ViewportRef::GetPlatformHandle>("platformHandle"),
		InstanceAccessor<&ViewportRef::GetCenter>("center"),
		InstanceAccessor<&ViewportRef::GetWorkPos>("workPos"),
		InstanceAccessor<&ViewportRef::GetWorkSize>("workSize")
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("ViewportRef", func);

    return exports;
}

Napi::Object ViewportRef::Create(Napi::Env env, ImGuiViewport* viewport)
{
    return constructor.New({FromPtr(env, viewport)});
}

ViewportRef::ViewportRef(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<ViewportRef>{info}
{
	m_viewport = static_cast<ImGuiViewport*>(AsPtr(info[0]));
}

Napi::Value ViewportRef::GetId(const Napi::CallbackInfo& info)
{
	return FromU32(info.Env(), m_viewport->ID);
}

Napi::Value ViewportRef::GetFlags(const Napi::CallbackInfo& info)
{
	return FromS32(info.Env(), m_viewport->Flags);
}

void ViewportRef::SetFlags(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_viewport->Flags = AsS32(value);
}

Napi::Value ViewportRef::GetPos(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_viewport->Pos);
}

void ViewportRef::SetPos(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_viewport->Pos = AsVec2(value);
}

Napi::Value ViewportRef::GetSize(const Napi::CallbackInfo& info)
{
	 return FromVec2(info.Env(), m_viewport->Size);
}

void ViewportRef::SetSize(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_viewport->Size = AsVec2(value);
}

Napi::Value ViewportRef::GetWorkOffsetMin(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_viewport->WorkOffsetMin);
}

void ViewportRef::SetWorkOffsetMin(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_viewport->WorkOffsetMin = AsVec2(value);
}

Napi::Value ViewportRef::GetWorkOffsetMax(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_viewport->WorkOffsetMax);
}

void ViewportRef::SetWorkOffsetMax(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_viewport->WorkOffsetMax = AsVec2(value);
}

Napi::Value ViewportRef::GetDPIScale(const Napi::CallbackInfo& info)
{
	return FromF32(info.Env(), m_viewport->DpiScale);
}

void ViewportRef::SetDPIScale(const Napi::CallbackInfo&, const Napi::Value& value)
{
	m_viewport->DpiScale = AsF32(value);
}

Napi::Value ViewportRef::GetParentViewportId(const Napi::CallbackInfo& info)
{
	return FromU32(info.Env(), m_viewport->ParentViewportId);
}

Napi::Value ViewportRef::GetPlatformHandle(const Napi::CallbackInfo& info)
{
    if (!m_viewport->PlatformHandle)
        return info.Env().Undefined();
    else
        return SDL::WindowRef::Create(info.Env(), static_cast<SDL_Window*>(m_viewport->PlatformHandle));
}

Napi::Value ViewportRef::GetCenter(const Napi::CallbackInfo& info)
{
    return FromVec2(info.Env(), m_viewport->GetCenter());
}

Napi::Value ViewportRef::GetWorkPos(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_viewport->GetWorkPos());
}

Napi::Value ViewportRef::GetWorkSize(const Napi::CallbackInfo& info)
{
	return FromVec2(info.Env(), m_viewport->GetWorkSize());
}

}
