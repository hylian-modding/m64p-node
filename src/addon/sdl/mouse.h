#pragma once

#include <napi.h>

namespace Addon::SDL::Mouse {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
