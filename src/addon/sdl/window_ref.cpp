#include "addon/imgui/param.h"
#include "addon/sdl/window_ref.h"
#include "addon/param.h"

#include <algorithm>

#include <SDL2/SDL_video.h>

using namespace Param;
using namespace Addon::ImGui_::Param;

namespace Addon::SDL {

using namespace Param;

Napi::FunctionReference WindowRef::constructor;

Napi::Object WindowRef::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "WindowRef", {
        InstanceMethod("getFlags", &WindowRef::GetFlags),
        InstanceMethod("getMaximumSize", &WindowRef::GetMaximumSize),
        InstanceMethod("getMinimumSize", &WindowRef::GetMinimumSize),
        InstanceMethod("getOpacity", &WindowRef::GetOpacity),
        InstanceMethod("getPosition", &WindowRef::GetPosition),
        InstanceMethod("getSize", &WindowRef::GetSize),
        InstanceMethod("getTitle", &WindowRef::GetTitle),
        InstanceMethod("hide", &WindowRef::Hide),
        InstanceMethod("maximize", &WindowRef::Maximize),
        InstanceMethod("minimize", &WindowRef::Minimize),
        InstanceMethod("raise", &WindowRef::Raise),
        InstanceMethod("restore", &WindowRef::Restore),
        InstanceMethod("setBordered", &WindowRef::SetBordered),
        InstanceMethod("setFullScreen", &WindowRef::SetFullScreen),
        InstanceMethod("setInputFocus", &WindowRef::SetInputFocus),
        InstanceMethod("setMaximumSize", &WindowRef::SetMaximumSize),
        InstanceMethod("setMinimumSize", &WindowRef::SetMinimumSize),
        InstanceMethod("setOpacity", &WindowRef::SetOpacity),
        InstanceMethod("setPosition", &WindowRef::SetPosition),
        InstanceMethod("setResizable", &WindowRef::SetResizable),
        InstanceMethod("setSize", &WindowRef::SetSize),
        InstanceMethod("setTitle", &WindowRef::SetTitle),
        InstanceMethod("show", &WindowRef::Show),
        InstanceMethod("equalsTo", &WindowRef::EqualsTo)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("WindowRef", func);

    return exports;
}

Napi::Object WindowRef::Create(Napi::Env env, SDL_Window* window)
{
    return constructor.New({FromPtr(env, window)});
}

WindowRef::WindowRef(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<WindowRef>{info}
{
	m_window = static_cast<SDL_Window*>(AsPtr(info[0]));
}

Napi::Value WindowRef::GetFlags(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), SDL_GetWindowFlags(m_window));
}

Napi::Value WindowRef::GetMaximumSize(const Napi::CallbackInfo& info)
{
    int x, y;
    SDL_GetWindowMaximumSize(m_window, &x, &y);

    return FromVec2(info.Env(), x, y);
}

Napi::Value WindowRef::GetMinimumSize(const Napi::CallbackInfo& info)
{
    int x, y;
    SDL_GetWindowMinimumSize(m_window, &x, &y);

    return FromVec2(info.Env(), x, y);
}

Napi::Value WindowRef::GetOpacity(const Napi::CallbackInfo& info)
{
    float v;
    SDL_GetWindowOpacity(m_window, &v);

    return FromF32(info.Env(), v);
}

Napi::Value WindowRef::GetPosition(const Napi::CallbackInfo& info)
{
    int x, y;
    SDL_GetWindowPosition(m_window, &x, &y);

    return FromVec2(info.Env(), x, y);
}

Napi::Value WindowRef::GetSize(const Napi::CallbackInfo& info)
{
    int x, y;
    SDL_GetWindowSize(m_window, &x, &y);

    return FromVec2(info.Env(), x, y);
}

Napi::Value WindowRef::GetTitle(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), SDL_GetWindowTitle(m_window));
}

Napi::Value WindowRef::Hide(const Napi::CallbackInfo& info)
{
    SDL_HideWindow(m_window);

    return info.Env().Undefined();
}

Napi::Value WindowRef::Maximize(const Napi::CallbackInfo& info)
{
    SDL_MaximizeWindow(m_window);

    return info.Env().Undefined();
}

Napi::Value WindowRef::Minimize(const Napi::CallbackInfo& info)
{
    SDL_MinimizeWindow(m_window);

    return info.Env().Undefined();
}

Napi::Value WindowRef::Raise(const Napi::CallbackInfo& info)
{
    SDL_RaiseWindow(m_window);

    return info.Env().Undefined();
}

Napi::Value WindowRef::Restore(const Napi::CallbackInfo& info)
{
    SDL_RestoreWindow(m_window);

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetBordered(const Napi::CallbackInfo& info)
{
    SDL_SetWindowBordered(m_window, AsBool(info[0]) ? SDL_TRUE : SDL_FALSE);

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetFullScreen(const Napi::CallbackInfo& info)
{
    SDL_SetWindowFullscreen(m_window, AsU32(info[0]));

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetInputFocus(const Napi::CallbackInfo& info)
{
    SDL_SetWindowInputFocus(m_window);

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetMaximumSize(const Napi::CallbackInfo& info)
{
    auto v = AsVec2(info[0]);
    SDL_SetWindowMaximumSize(m_window, static_cast<int>(v.x), static_cast<int>(v.y));

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetMinimumSize(const Napi::CallbackInfo& info)
{
    auto v = AsVec2(info[0]);
    SDL_SetWindowMinimumSize(m_window, static_cast<int>(v.x), static_cast<int>(v.y));

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetOpacity(const Napi::CallbackInfo& info)
{
    SDL_SetWindowOpacity(m_window, std::clamp(AsF32(info[0]), 0.0f, 1.0f));

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetPosition(const Napi::CallbackInfo& info)
{
    auto v = AsVec2(info[0]);
    SDL_SetWindowPosition(m_window, static_cast<int>(v.x), static_cast<int>(v.y));

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetResizable(const Napi::CallbackInfo& info)
{
    SDL_SetWindowResizable(m_window, AsBool(info[0]) ? SDL_TRUE : SDL_FALSE);

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetSize(const Napi::CallbackInfo& info)
{
    auto v = AsVec2(info[0]);
    SDL_SetWindowSize(m_window, static_cast<int>(v.x), static_cast<int>(v.y));

    return info.Env().Undefined();
}

Napi::Value WindowRef::SetTitle(const Napi::CallbackInfo& info)
{
    SDL_SetWindowTitle(m_window, AsStrUtf8(info[0]).c_str());

    return info.Env().Undefined();
}

Napi::Value WindowRef::Show(const Napi::CallbackInfo& info)
{
    SDL_ShowWindow(m_window);

    return info.Env().Undefined();
}

Napi::Value WindowRef::EqualsTo(const Napi::CallbackInfo& info)
{
    auto other = Unwrap(AsObject(info[0]))->m_window;

    return FromBool(info.Env(), m_window == other);
}

}
