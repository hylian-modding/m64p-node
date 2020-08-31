#include "addon/mixer/music.h"
#include "addon/mixer/sound.h"

namespace Addon::Mixer {

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    Music::Init(env, exports);
    Sound::Init(env, exports);

    return exports;
}

}
