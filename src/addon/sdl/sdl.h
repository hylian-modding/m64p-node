#pragma once

#include <napi.h>

namespace Addon::SDL {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
