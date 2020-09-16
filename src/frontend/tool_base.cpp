#include "frontend/tool_base.h"
#include "imgui/imgui.h"
#include "sdl/window.h"

#include <SDL2/SDL_video.h>

namespace Frontend {

bool ToolBase::IsOpen()
{
    return m_open;
}

void ToolBase::Open()
{
    if (m_open)
        m_raise = true;

    m_open = true;
}

void ToolBase::Close()
{
    m_open = false;
    OnClosed.fire();
}

int ToolBase::GetWindowFlags(SDL::Window& main_win)
{
    int window_flags = ImGuiWindowFlags_NoCollapse;

    // if (m_window != main_win.Get())
    //     window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

    return window_flags;
}

void ToolBase::CommonBehavior()
{
    if (ImGui::GetWindowViewport()->PlatformHandle)
        m_window = static_cast<SDL_Window*>(ImGui::GetWindowViewport()->PlatformHandle);
    else
        m_window = nullptr;

    if (m_raise && m_window) {
        SDL_RaiseWindow(m_window);
        SDL_SetWindowInputFocus(m_window);
        m_raise = false;
    }

    if (!m_open)
        OnClosed.fire();
}

}
