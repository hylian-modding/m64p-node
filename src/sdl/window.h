#pragma once

#include "common/types.h"

#include <memory>
#include <string>

struct SDL_Window;

namespace SDL {

class Surface;

class Window {
public:
	Window();
	Window(const std::string& title, int x, int y, int width, int height, u32 flags);

	SDL_Window* Get() const;
    void SetTitle(const std::string& title);
    void SetIcon(const Surface& icon);
    void Swap();
    void ToggleFullScreen();
    bool IsFullScreen();
    u32 GetId();
    bool HasInputFocus() const;
    void Show();
    void Hide();
    void Raise();
    int GetWidth() const;
    int GetHeight() const;

private:
	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> m_ptr;
};

}
