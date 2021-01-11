#include "addon/frontend/frontend.h"
#include "addon/gfx/gfx.h"
#include "addon/imgui/imgui.h"
#include "addon/m64p/m64p.h"
#include "addon/sdl/sdl.h"
#include "addon/yaz0/yaz0.h"

Napi::Object BuildModule(Napi::Env env, Napi::Object exports)
{
    exports.Set("Gfx", Addon::Gfx::BuildExports(env, Napi::Object::New(env)));
    exports.Set("SDL", Addon::SDL::BuildExports(env, Napi::Object::New(env)));
    exports.Set("ImGui", Addon::ImGui_::BuildExports(env, Napi::Object::New(env)));
    exports.Set("M64p", Addon::M64P::BuildExports(env, Napi::Object::New(env)));
    exports.Set("Yaz0", Addon::Yaz0::BuildExports(env, Napi::Object::New(env)));
    exports.Set("Frontend", Addon::Frontend::BuildExports(env, Napi::Object::New(env)));

    return exports;
}

NODE_API_MODULE(NODE_GYP_M64P_NODE, BuildModule)
