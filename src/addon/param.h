#pragma once

#include "common/types.h"

#include <filesystem>
#include <string>

#include <napi.h>

namespace Param {

inline Napi::Object AsObject(const Napi::Value& value)
{
    return value.As<Napi::Object>();
}

inline Napi::Array AsArray(const Napi::Value& value)
{
    return value.As<Napi::Array>();
}

inline u16 AsU16(const Napi::Value& value)
{
    return static_cast<u16>(value.As<Napi::Number>().Uint32Value());
}

inline s32 AsS32(const Napi::Value& value)
{
    return value.As<Napi::Number>().Int32Value();
}

inline s32 AsS32Or(const Napi::Value& value, s32 default_value)
{
    return value.IsUndefined() ? default_value : AsS32(value);
}

inline Napi::Value FromS32(Napi::Env env, s32 value)
{
    return Napi::Number::New(env, value);
}

inline u32 AsU32(const Napi::Value& value)
{
    return value.As<Napi::Number>().Uint32Value();
}

inline Napi::Value FromU32(Napi::Env env, u32 value)
{
    return Napi::Number::New(env, value);
}

inline s64 AsS64(const Napi::Value& value)
{
    return value.As<Napi::Number>().Int64Value();
}

inline f32 AsF32(const Napi::Value& value)
{
    return value.As<Napi::Number>().FloatValue();
}

inline f32 AsF32Or(const Napi::Value& value, f32 default_value)
{
    return value.IsUndefined() ? default_value : AsF32(value);
}

inline Napi::Value FromF32(Napi::Env env, f32 value)
{
    return Napi::Number::New(env, value);
}

inline f64 AsF64(const Napi::Value& value)
{
    return value.As<Napi::Number>().DoubleValue();
}

inline f64 AsF64Or(const Napi::Value& value, f64 default_value)
{
    return value.IsUndefined() ? default_value : AsF64(value);
}

inline Napi::Value FromF64(Napi::Env env, f64 value)
{
    return Napi::Number::New(env, value);
}

inline bool AsBool(const Napi::Value& value)
{
    return value.As<Napi::Boolean>().Value();
}

inline bool AsBoolOr(const Napi::Value& value, bool default_value)
{
    return value.IsUndefined() ? default_value : AsBool(value);
}

inline bool ToBool(const Napi::Value& value)
{
    return value.ToBoolean();
}

inline Napi::Value FromBool(Napi::Env env, bool value)
{
    return Napi::Boolean::New(env, value);
}

inline void* AsPtr(const Napi::Value& value)
{
    return reinterpret_cast<void*>(value.As<Napi::Number>().Int64Value());
}

inline Napi::Value FromPtr(Napi::Env env, void* value)
{
    return Napi::Number::New(env, static_cast<double>(reinterpret_cast<s64>(value)));
}

inline std::string AsStrUtf8(const Napi::Value& value)
{
    return value.As<Napi::String>().Utf8Value();
}

inline std::string AsStrUtf8Or(const Napi::Value& value, const std::string& default_value)
{
    return value.IsUndefined() ? default_value : AsStrUtf8(value);
}

inline Napi::Value FromStrUtf8(Napi::Env env, std::string value)
{
    return Napi::String::New(env, std::move(value));
}

inline std::filesystem::path AsPath(const Napi::Value& value)
{
#ifdef _WIN32
	return value.As<Napi::String>().Utf16Value();
#else
	return value.As<Napi::String>().Utf8Value();
#endif
}

char* GetStrUtf8(std::size_t index);
void LoadStrUtf8(std::size_t index, const Napi::Value& value);
void LoadStrUtf8Or(std::size_t index, const Napi::Value& value, const char* default_value);

}
