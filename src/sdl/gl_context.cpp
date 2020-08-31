#include "sdl/error.h"
#include "sdl/gl_context.h"
#include "sdl/window.h"

#include <SDL2/SDL_video.h>

namespace SDL {

GLContext::GLContext() :
	m_ptr{nullptr, SDL_GL_DeleteContext}
{}

GLContext::GLContext(Window& window) :
	GLContext{}
{
	m_ptr.reset(SDL_GL_CreateContext(window.Get()));

	if (!m_ptr)
		throw Error{"SDL_GL_CreateContext"};
}

SDL_GLContext GLContext::Get() const
{
    return m_ptr.get();
}

void GLContext::MakeCurrent(Window& window)
{
	SDL_GL_MakeCurrent(window.Get(), Get());
}

void GLContext::MakeCurrentNone()
{
    SDL_GL_MakeCurrent(nullptr, nullptr);
}

}
