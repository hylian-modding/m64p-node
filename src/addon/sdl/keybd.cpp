#include "addon/param.h"

#include <SDL2/SDL_keyboard.h>

using namespace Param;

namespace Addon::SDL::Keybd {

Napi::Value GetKeyFromName(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromS32(info.Env(), SDL_GetKeyFromName(GetStrUtf8(0)));
}

Napi::Value GetKeyFromScancode(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(AsS32(info[0]))));
}

Napi::Value GetKeyName(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), SDL_GetKeyName(static_cast<SDL_Keycode>(AsS32(info[0]))));
}

Napi::Value GetKeyState(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), SDL_GetKeyboardState(nullptr)[AsS32(info[0])]);
}

Napi::Value GetModState(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), SDL_GetModState());
}

Napi::Value GetScancodeFromKey(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(AsS32(info[0]))));
}

Napi::Value GetScancodeFromName(const Napi::CallbackInfo& info)
{
    LoadStrUtf8(0, info[0]);

    return FromS32(info.Env(), SDL_GetScancodeFromName(GetStrUtf8(0)));
}

Napi::Value GetScancodeName(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), SDL_GetScancodeName(static_cast<SDL_Scancode>(AsS32(info[0]))));
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    exports.Set("getKeyFromName", Napi::Function::New(env, GetKeyFromName));
    exports.Set("getKeyFromScancode", Napi::Function::New(env, GetKeyFromScancode));
    exports.Set("getKeyName", Napi::Function::New(env, GetKeyName));
    exports.Set("getKeyState", Napi::Function::New(env, GetKeyState));
    exports.Set("getModState", Napi::Function::New(env, GetModState));
    exports.Set("getScancodeFromKey", Napi::Function::New(env, GetScancodeFromKey));
    exports.Set("getScancodeFromName", Napi::Function::New(env, GetScancodeFromName));
    exports.Set("getScancodeName", Napi::Function::New(env, GetScancodeName));

    return exports;
}

}
