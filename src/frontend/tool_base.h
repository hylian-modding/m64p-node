#pragma once

#include "sdl/fwd.h"

#include <nano_signal_slot.hpp>

union SDL_Event;
struct SDL_Window;

namespace Frontend {

class ToolBase {
public:
    Nano::Signal<void()> OnClosed;

    bool IsOpen();
    void Open();
    void Close();

protected:
    bool m_open{};
    bool m_raise{};
    SDL_Window* m_window{};

    int GetWindowFlags(SDL::Window& main_win);
    void CommonBehavior();
};

}
