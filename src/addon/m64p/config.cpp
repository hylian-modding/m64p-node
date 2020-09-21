#include "addon/m64p/config_section.h"
#include "addon/param.h"
#include "addon/safe_call.h"
#include "frontend/app.h"

#include <fmt/format.h>

using namespace Param;

namespace Addon::M64P::Config {

inline ::M64P::Core& GetCore()
{
    return Frontend::App::GetInstance().GetCore();
}

Napi::Value SaveFile(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().ConfigSaveFile();

        return info.Env().Undefined();
    });
}

Napi::Value HasUnsavedChanges(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), GetCore().ConfigHasUnsavedChanges());
}

Napi::Value ListSections(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        auto ret = Napi::Array::New(info.Env());
        auto names = GetCore().ConfigListSections();

		for (std::size_t i{}; i < names.size(); ++i)
			ret.Set(static_cast<u32>(i), FromStrUtf8(info.Env(), names[i]));

		return ret;
    });
}

Napi::Value OpenSection(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
		return Section::constructor.New({info[0]});
    });
}

Napi::Value GetSharedDataFilePath(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        auto file = AsPath(info[0]).string();
        auto path = GetCore().GetSharedDataFilePath(file).string();

        if (path.empty())
            throw std::runtime_error(fmt::format("Shared data file not found '{}'", file));

        return FromStrUtf8(info.Env(), path);
    });
}

Napi::Value GetUserConfigPath(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), GetCore().GetUserConfigPath().string());
}

Napi::Value GetUserDataPath(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), GetCore().GetUserDataPath().string());
}

Napi::Value GetUserCachePath(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), GetCore().GetUserCachePath().string());
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    Section::Init(env, exports);

    exports.Set("saveFile", Napi::Function::New(env, SaveFile));
    exports.Set("hasUnsavedChanges", Napi::Function::New(env, HasUnsavedChanges));
    exports.Set("listSections", Napi::Function::New(env, ListSections));
    exports.Set("openSection", Napi::Function::New(env, OpenSection));

    exports.Set("getSharedDataFilePath", Napi::Function::New(env, GetSharedDataFilePath));
    exports.Set("getUserConfigPath", Napi::Function::New(env, GetUserConfigPath));
    exports.Set("getUserDataPath", Napi::Function::New(env, GetUserDataPath));
    exports.Set("getUserCachePath", Napi::Function::New(env, GetUserCachePath));

    return exports;
}

}
