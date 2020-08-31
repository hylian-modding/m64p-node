#include "addon/imgui/param.h"
#include "addon/sdl/window_ref.h"
#include "addon/param.h"

#include <SDL2/SDL_mouse.h>

using namespace Param;
using namespace Addon::ImGui_::Param;

namespace Addon::SDL::Mouse {

Napi::Value Capture(const Napi::CallbackInfo& info)
{
    SDL_CaptureMouse(AsBool(info[0]) ? SDL_TRUE : SDL_FALSE);

    return info.Env().Undefined();
}

Napi::Value GetButtonMask(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), SDL_BUTTON(AsU32(info[0])));
}

Napi::Value GetGlobalPosition(const Napi::CallbackInfo& info)
{
    int x, y;
    SDL_GetGlobalMouseState(&x, &y);

    return FromVec2(info.Env(), x, y);
}

Napi::Value GetGlobalButtons(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), SDL_GetGlobalMouseState(nullptr, nullptr));
}

Napi::Value GetPosition(const Napi::CallbackInfo& info)
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    return FromVec2(info.Env(), x, y);
}

Napi::Value GetButtons(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), SDL_GetMouseState(nullptr, nullptr));
}

Napi::Value GetRelativeMode(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), SDL_GetRelativeMouseMode());
}

Napi::Value GetRelativePosition(const Napi::CallbackInfo& info)
{
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);

    return FromVec2(info.Env(), x, y);
}

Napi::Value GetRelativeButtons(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), SDL_GetRelativeMouseState(nullptr, nullptr));
}

Napi::Value SetRelativeMode(const Napi::CallbackInfo& info)
{
    SDL_SetRelativeMouseMode(AsBool(info[0]) ? SDL_TRUE : SDL_FALSE);

    return info.Env().Undefined();
}

Napi::Value WarpGlobal(const Napi::CallbackInfo& info)
{
    auto v = AsVec2(info[0]);
    SDL_WarpMouseGlobal(static_cast<int>(v.x), static_cast<int>(v.y));

    return info.Env().Undefined();
}

Napi::Value WarpInWindow(const Napi::CallbackInfo& info)
{
    auto w = WindowRef::Unwrap(AsObject(info[0]))->m_window;
    auto v = AsVec2(info[1]);
    SDL_WarpMouseInWindow(w, static_cast<int>(v.x), static_cast<int>(v.y));

    return info.Env().Undefined();
}

Napi::Value ShowCursor(const Napi::CallbackInfo& info)
{
    SDL_ShowCursor(1);

    return info.Env().Undefined();
}

Napi::Value HideCursor(const Napi::CallbackInfo& info)
{
    SDL_ShowCursor(0);

    return info.Env().Undefined();
}

Napi::Value IsCursorVisible(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), SDL_ShowCursor(-1));
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    exports.Set("capture", Napi::Function::New(env, Capture));
    exports.Set("getButtonMask", Napi::Function::New(env, GetButtonMask));
    exports.Set("getGlobalPosition", Napi::Function::New(env, GetGlobalPosition));
    exports.Set("getGlobalButtons", Napi::Function::New(env, GetGlobalButtons));
    exports.Set("getPosition", Napi::Function::New(env, GetPosition));
    exports.Set("getButtons", Napi::Function::New(env, GetButtons));
    exports.Set("getRelativeMode", Napi::Function::New(env, GetRelativeMode));
    exports.Set("getRelativePosition", Napi::Function::New(env, GetRelativePosition));
    exports.Set("getRelativeButtons", Napi::Function::New(env, GetRelativeButtons));
    exports.Set("setRelativeMode", Napi::Function::New(env, SetRelativeMode));
    exports.Set("warpGlobal", Napi::Function::New(env, WarpGlobal));
    exports.Set("warpInWindow", Napi::Function::New(env, WarpInWindow));
    exports.Set("showCursor", Napi::Function::New(env, ShowCursor));
    exports.Set("hideCursor", Napi::Function::New(env, HideCursor));
    exports.Set("isCursorVisible", Napi::Function::New(env, IsCursorVisible));

    return exports;
}

}
