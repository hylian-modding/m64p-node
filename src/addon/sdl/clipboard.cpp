#include "addon/param.h"

#include <SDL2/SDL_clipboard.h>

using namespace Param;

namespace Addon::SDL::Clipboard {

Napi::Value GetText(const Napi::CallbackInfo& info)
{
    auto v = SDL_GetClipboardText();

    return FromStrUtf8(info.Env(), !v ? "" : v);
}

Napi::Value HasText(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), SDL_HasClipboardText());
}

Napi::Value SetText(const Napi::CallbackInfo& info)
{
    SDL_SetClipboardText(AsStrUtf8(info[0]).c_str());

    return info.Env().Undefined();
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    exports.Set("getText", Napi::Function::New(env, GetText));
    exports.Set("hasText", Napi::Function::New(env, HasText));
    exports.Set("setText", Napi::Function::New(env, SetText));

    return exports;
}

}
