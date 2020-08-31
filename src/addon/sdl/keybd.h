#pragma once

#include <napi.h>

namespace Addon::SDL::Keybd {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
