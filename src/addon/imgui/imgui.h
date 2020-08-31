#pragma once

#include <napi.h>

namespace Addon::ImGui_ {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
