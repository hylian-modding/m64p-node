#include "sdl/error.h"
#include "sdl/sdl_init.h"

#include <SDL2/SDL.h>

namespace SDL {

SDLInit::~SDLInit()
{
    if (m_init)
        SDL_Quit();
}

void SDLInit::Init(u32 flags)
{
    if (m_init)
        SDL_Quit();

    m_init = false;

    if (SDL_Init(flags) < 0)
        throw Error{"SDL_Init"};

    m_init = true;
}

void SDLInit::Quit()
{
    if (m_init) {
        SDL_Quit();
        m_init = false;
    }
}

}
