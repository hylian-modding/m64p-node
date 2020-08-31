#include "addon/sdl/clipboard.h"
#include "addon/sdl/keybd.h"
#include "addon/sdl/mouse.h"
#include "addon/sdl/window_ref.h"

namespace Addon::SDL {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    WindowRef::Init(env, exports);

    exports.Set("Keybd", Keybd::BuildExports(env, Napi::Object::New(env)));
    exports.Set("Mouse", Mouse::BuildExports(env, Napi::Object::New(env)));
    exports.Set("Clipboard", Clipboard::BuildExports(env, Napi::Object::New(env)));

    return exports;
}

}
