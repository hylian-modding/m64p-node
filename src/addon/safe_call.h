#pragma once

#include "common/logger.h"

#include <napi.h>

template <class F, class...Ts>
Napi::Value SafeCall(Napi::Env env, const F& f)
{
    try {
        return f();
    }
    catch (const std::exception& e) {
        Logger::Log(LogCategory::Error, "M64p binding", e.what());
        throw Napi::Error::New(env, e.what());
    }
    catch (...) {
        Logger::Log(LogCategory::Error, "M64p binding", "Unknown error");
        throw Napi::Error::New(env, "Unknown error");
    }
}
