#pragma once

#include <napi.h>

template <class F, class...Ts>
Napi::Value SafeCall(Napi::Env env, const F& f)
{
    try {
        return f();
    }
    catch (const std::exception& e) {
        throw Napi::Error::New(env, e.what());
    }
    catch (...) {
        throw Napi::Error::New(env, "unknown error");
    }
}
