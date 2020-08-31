#pragma once

#include <napi.h>

namespace Addon::Gfx {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
