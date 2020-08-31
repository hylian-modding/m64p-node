#pragma once

#include <napi.h>

namespace Addon::Yaz0 {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
