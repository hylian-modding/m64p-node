#include "sdl/error.h"
#include "sdl/mix_device.h"

#include <SDL2/SDL_mixer.h>

namespace SDL {

MixDevice::~MixDevice()
{
    if (m_init)
        Mix_CloseAudio();
}

void MixDevice::Open(int frequency, u16 format, int channels, int chunk_size)
{
    if (m_init)
        Mix_CloseAudio();

    m_init = false;

    if (Mix_OpenAudio(frequency, format, channels, chunk_size) < 0)
        throw Error{"Mix_OpenAudio"};

    m_init = true;
}

void MixDevice::Close()
{
    if (m_init) {
        Mix_CloseAudio();
        m_init = false;
    }
}

}
