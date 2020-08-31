#include "sdl/error.h"
#include "sdl/ttf_init.h"

#include <SDL2/SDL_ttf.h>

namespace SDL {

TTFInit::~TTFInit()
{
    if (m_init)
        TTF_Quit();
}

void TTFInit::Init()
{
    if (m_init)
        TTF_Quit();

    m_init = false;

    if (TTF_Init() < 0)
        throw Error{"TTF_Init"};

    m_init = true;
}

void TTFInit::Quit()
{
    if (m_init) {
        TTF_Quit();
        m_init = false;
    }
}

}
