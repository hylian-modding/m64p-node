#pragma once

#include <napi.h>

namespace Addon::Mixer {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
