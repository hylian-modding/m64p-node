#include "sdl/error.h"
#include "sdl/surface.h"
#include "sdl/window.h"

#include <SDL2/SDL_video.h>

namespace SDL {

Window::Window() :
	m_ptr{nullptr, SDL_DestroyWindow}
{}

Window::Window(const std::string& title, int x, int y, int width, int height, u32 flags) :
	Window{}
{
	m_ptr.reset(SDL_CreateWindow(title.c_str(), x, y, width, height, flags));

	if (!m_ptr)
		throw Error{"SDL_CreateWindow"};
}

SDL_Window* Window::Get() const
{
    return m_ptr.get();
}

void Window::SetTitle(const std::string& title)
{
    SDL_SetWindowTitle(Get(), title.c_str());
}

void Window::SetIcon(const SDL::Surface& icon)
{
    SDL_SetWindowIcon(Get(), icon.Get());
}

void Window::Swap()
{
    SDL_GL_SwapWindow(Get());
}

void Window::ToggleFullScreen()
{
    SDL_SetWindowFullscreen(Get(), SDL_GetWindowFlags(Get()) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_SetWindowInputFocus(Get());
}

bool Window::IsFullScreen()
{
    return (SDL_GetWindowFlags(Get()) & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP;
}

u32 Window::GetId()
{
    return SDL_GetWindowID(Get());
}

bool Window::HasInputFocus() const
{
    return (SDL_GetWindowFlags(Get()) & SDL_WINDOW_INPUT_FOCUS) == SDL_WINDOW_INPUT_FOCUS;
}

void Window::Show()
{
    SDL_ShowWindow(Get());
}

void Window::Hide()
{
    SDL_HideWindow(Get());
}

void Window::Raise()
{
    SDL_RaiseWindow(Get());
}

int Window::GetWidth() const
{
    int w;
    SDL_GetWindowSize(Get(), &w, nullptr);

    return w;
}

int Window::GetHeight() const
{
    int h;
    SDL_GetWindowSize(Get(), nullptr, &h);

    return h;
}

}
