#include "sdl/error.h"
#include "sdl/mix_init.h"

#include <SDL2/SDL_mixer.h>

namespace SDL {

MixInit::~MixInit()
{
    if (m_init)
        Mix_Quit();
}

void MixInit::Init(int flags)
{
    if (m_init)
        Mix_Quit();

    m_init = false;

    int ret = Mix_Init(flags);

    if (ret == 0 && flags != 0)
        throw Error{"Mix_Init:Failure"};
    else if (ret != flags)
        throw Error{"Mix_Init:Flags"};

    m_init = true;
}

void MixInit::Quit()
{
    if (m_init) {
        Mix_Quit();
        m_init = false;
    }
}

}
