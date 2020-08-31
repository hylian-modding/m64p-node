#include "sdl/error.h"
#include "sdl/img_init.h"

#include <SDL2/SDL_image.h>

namespace SDL {

IMGInit::~IMGInit()
{
    if (m_init)
        IMG_Quit();
}

void IMGInit::Init(int flags)
{
    if (m_init)
        IMG_Quit();

    m_init = false;

    int ret = IMG_Init(flags);

    if (ret == 0 && flags != 0)
        throw Error{"IMG_Init:Failure"};
    else if (ret != flags)
        throw Error{"IMG_Init:Flags"};

    m_init = true;
}

void IMGInit::Quit()
{
    if (m_init) {
        IMG_Quit();
        m_init = false;
    }
}

}
