#pragma once

#include <napi.h>

namespace Addon::M64P::Config {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
