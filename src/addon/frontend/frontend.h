#pragma once

#include <napi.h>

namespace Addon::Frontend {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
