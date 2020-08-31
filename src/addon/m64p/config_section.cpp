#include "addon/m64p/config_section.h"
#include "addon/param.h"
#include "addon/safe_call.h"
#include "frontend/app.h"

using namespace Param;

namespace Addon::M64P::Config {

inline ::M64P::Core& GetCore()
{
    return Frontend::App::GetInstance().GetCore();
}

Napi::FunctionReference Section::constructor;

Napi::Object Section::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope{env};

    Napi::Function func =
    DefineClass(env, "Section", {
        InstanceMethod("getName", &Section::GetName),
        InstanceMethod("listParams", &Section::ListParams),
        InstanceMethod("save", &Section::Save),
        InstanceMethod("hasUnsavedChanges", &Section::HasUnsavedChanges),
        InstanceMethod("erase", &Section::Erase),
        InstanceMethod("revertChanges", &Section::RevertChanges),

        InstanceMethod("getHelp", &Section::GetHelp),
        InstanceMethod("setHelp", &Section::SetHelp),
        InstanceMethod("getType", &Section::GetType),

        InstanceMethod("setDefaultInt", &Section::SetDefaultInt),
        InstanceMethod("setDefaultFloat", &Section::SetDefaultFloat),
        InstanceMethod("setDefaultBool", &Section::SetDefaultBool),
        InstanceMethod("setDefaultString", &Section::SetDefaultString),

        InstanceMethod("getInt", &Section::GetInt),
        InstanceMethod("getIntOr", &Section::GetIntOr),
        InstanceMethod("setInt", &Section::SetInt),
        InstanceMethod("getFloat", &Section::GetFloat),
        InstanceMethod("getFloatOr", &Section::GetFloatOr),
        InstanceMethod("setFloat", &Section::SetFloat),
        InstanceMethod("getBool", &Section::GetBool),
        InstanceMethod("getBoolOr", &Section::GetBoolOr),
        InstanceMethod("setBool", &Section::SetBool),
        InstanceMethod("getString", &Section::GetString),
        InstanceMethod("getStringOr", &Section::GetStringOr),
        InstanceMethod("setString", &Section::SetString)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Section", func);

    return exports;
}

Section::Section(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Section>{info}
{
    SafeCall(info.Env(), [this, &info]() {
        m_section = std::make_unique<::M64P::Core::ConfigSection>(GetCore().ConfigOpenSection(AsStrUtf8(info[0])));

        return info.Env().Undefined();
    });
}

Napi::Value Section::GetName(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), m_section->GetName());
}

Napi::Value Section::ListParams(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        auto ret = Napi::Array::New(info.Env());
        auto params = m_section->ListParams();

        for (std::size_t i{}; i < params.size(); ++i) {
            auto obj = Napi::Object::New(info.Env());
            obj.Set("name", FromStrUtf8(info.Env(), params[i].name));
            obj.Set("type", FromS32(info.Env(), params[i].type));

            ret.Set(static_cast<u32>(i), obj);
        }

        return ret;
    });
}

Napi::Value Section::Save(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->Save();

        return info.Env().Undefined();
    });
}

Napi::Value Section::HasUnsavedChanges(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), m_section->HasUnsavedChanges());
}

Napi::Value Section::Erase(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->Erase();

        return info.Env().Undefined();
    });
}

Napi::Value Section::RevertChanges(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->RevertChanges();

        return info.Env().Undefined();
    });
}

Napi::Value Section::GetHelp(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), m_section->GetHelp(AsStrUtf8(info[0])));
}

Napi::Value Section::SetHelp(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->SetHelp(AsStrUtf8(info[0]), AsStrUtf8(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value Section::GetType(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        return FromS32(info.Env(), m_section->GetType(AsStrUtf8(info[0])));
    });
}

Napi::Value Section::SetDefaultInt(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->SetDefaultInt(AsStrUtf8(info[0]), AsS32(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value Section::SetDefaultFloat(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->SetDefaultFloat(AsStrUtf8(info[0]), AsF32(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value Section::SetDefaultBool(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->SetDefaultBool(AsStrUtf8(info[0]), AsBool(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value Section::SetDefaultString(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->SetDefaultString(AsStrUtf8(info[0]), AsStrUtf8(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value Section::GetInt(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), m_section->GetInt(AsStrUtf8(info[0])));
}

Napi::Value Section::GetIntOr(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), m_section->GetIntOr(AsStrUtf8(info[0]), AsS32(info[1])));
}

Napi::Value Section::SetInt(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->SetInt(AsStrUtf8(info[0]), AsS32(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value Section::GetFloat(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), m_section->GetFloat(AsStrUtf8(info[0])));
}

Napi::Value Section::GetFloatOr(const Napi::CallbackInfo& info)
{
    return FromF32(info.Env(), m_section->GetFloatOr(AsStrUtf8(info[0]), AsF32(info[1])));
}

Napi::Value Section::SetFloat(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->SetFloat(AsStrUtf8(info[0]), AsF32(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value Section::GetBool(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), m_section->GetBool(AsStrUtf8(info[0])));
}

Napi::Value Section::GetBoolOr(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), m_section->GetBoolOr(AsStrUtf8(info[0]), AsBool(info[1])));
}

Napi::Value Section::SetBool(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->SetBool(AsStrUtf8(info[0]), AsBool(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value Section::GetString(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), m_section->GetString(AsStrUtf8(info[0])));
}

Napi::Value Section::GetStringOr(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), m_section->GetStringOr(AsStrUtf8(info[0]), AsStrUtf8(info[1])));
}

Napi::Value Section::SetString(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [this, &info]() {
        m_section->SetString(AsStrUtf8(info[0]), AsStrUtf8(info[1]));

        return info.Env().Undefined();
    });
}

}
