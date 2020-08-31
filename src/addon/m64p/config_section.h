#pragma once

#include "m64p/core.h"

#include <memory>

#include <napi.h>

namespace Addon::M64P::Config {

struct Section : Napi::ObjectWrap<Section> {
    static Napi::FunctionReference constructor;
    std::unique_ptr<::M64P::Core::ConfigSection> m_section;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    Section(const Napi::CallbackInfo& info);

    Napi::Value GetName(const Napi::CallbackInfo& info);
    Napi::Value ListParams(const Napi::CallbackInfo& info);
    Napi::Value Save(const Napi::CallbackInfo& info);
    Napi::Value HasUnsavedChanges(const Napi::CallbackInfo& info);
    Napi::Value Erase(const Napi::CallbackInfo& info);
    Napi::Value RevertChanges(const Napi::CallbackInfo& info);

    Napi::Value GetHelp(const Napi::CallbackInfo& info);
    Napi::Value SetHelp(const Napi::CallbackInfo& info);
    Napi::Value GetType(const Napi::CallbackInfo& info);

    Napi::Value SetDefaultInt(const Napi::CallbackInfo& info);
    Napi::Value SetDefaultFloat(const Napi::CallbackInfo& info);
    Napi::Value SetDefaultBool(const Napi::CallbackInfo& info);
    Napi::Value SetDefaultString(const Napi::CallbackInfo& info);

    Napi::Value GetInt(const Napi::CallbackInfo& info);
    Napi::Value GetIntOr(const Napi::CallbackInfo& info);
    Napi::Value SetInt(const Napi::CallbackInfo& info);
    Napi::Value GetFloat(const Napi::CallbackInfo& info);
    Napi::Value GetFloatOr(const Napi::CallbackInfo& info);
    Napi::Value SetFloat(const Napi::CallbackInfo& info);
    Napi::Value GetBool(const Napi::CallbackInfo& info);
    Napi::Value GetBoolOr(const Napi::CallbackInfo& info);
    Napi::Value SetBool(const Napi::CallbackInfo& info);
    Napi::Value GetString(const Napi::CallbackInfo& info);
    Napi::Value GetStringOr(const Napi::CallbackInfo& info);
    Napi::Value SetString(const Napi::CallbackInfo& info);
};

}
