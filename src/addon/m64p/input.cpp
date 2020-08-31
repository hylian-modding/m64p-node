#include "addon/param.h"
#include "frontend/app.h"
#include "frontend/input.h"

using namespace Param;

namespace Addon::M64P::Input {

inline ::M64P::Core& GetCore()
{
    return Frontend::App::GetInstance().GetCore();
}

inline Frontend::Input& GetInput()
{
    return Frontend::App::GetInstance().GetInput();
}

Napi::Value GetAxis(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), GetInput().GetAxis(AsU32(info[0]), AsU32(info[1])));
}

Napi::Value SetAxis(const Napi::CallbackInfo& info)
{
    GetInput().SetAxis(AsU32(info[0]), AsU32(info[1]), AsF32(info[2]));

    return info.Env().Undefined();
}

Napi::Value SetAxisVi(const Napi::CallbackInfo& info)
{
    GetInput().SetAxisVi(AsU32(info[0]), AsU32(info[1]), AsF32(info[2]));

    return info.Env().Undefined();
}

Napi::Value GetButton(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), GetInput().GetButton(AsU32(info[0]), AsU32(info[1])));
}

Napi::Value GetButtonDown(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), GetInput().GetButtonDown(AsU32(info[0]), AsU32(info[1])));
}

Napi::Value GetButtonUp(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), GetInput().GetButtonUp(AsU32(info[0]), AsU32(info[1])));
}

Napi::Value SetButton(const Napi::CallbackInfo& info)
{
    GetInput().SetButton(AsU32(info[0]), AsU32(info[1]), AsBool(info[2]));

    return info.Env().Undefined();
}

Napi::Value SetButtonDown(const Napi::CallbackInfo& info)
{
    GetInput().SetButtonDown(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value GetPluginFilter(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().ContGetPluginFilter(AsS32(info[0])));
}

Napi::Value SetPluginFilter(const Napi::CallbackInfo& info)
{
    GetCore().ContSetPluginFilter(AsS32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    exports.Set("getAxis", Napi::Function::New(env, GetAxis));
    exports.Set("setAxis", Napi::Function::New(env, SetAxis));
    exports.Set("setAxisOneFrame", Napi::Function::New(env, SetAxisVi));
    exports.Set("getButton", Napi::Function::New(env, GetButton));
    exports.Set("getButtonDown", Napi::Function::New(env, GetButtonDown));
    exports.Set("getButtonUp", Napi::Function::New(env, GetButtonUp));
    exports.Set("setButton", Napi::Function::New(env, SetButton));
    exports.Set("setButtonDown", Napi::Function::New(env, SetButtonDown));
    exports.Set("getPluginFilter", Napi::Function::New(env, GetPluginFilter));
    exports.Set("setPluginFilter", Napi::Function::New(env, SetPluginFilter));

    return exports;
}

}
