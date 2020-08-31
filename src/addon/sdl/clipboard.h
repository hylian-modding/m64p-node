#pragma once

#include <napi.h>

namespace Addon::SDL::Clipboard {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports);

}
