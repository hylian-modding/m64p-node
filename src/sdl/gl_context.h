#pragma once

#include <memory>

using SDL_GLContext = void*;

namespace SDL {

class Window;

class GLContext {
public:
	GLContext();
	explicit GLContext(Window& window);

	SDL_GLContext Get() const;
	void MakeCurrent(Window& window);
    static void MakeCurrentNone();

private:
	std::unique_ptr<void, void(*)(SDL_GLContext)> m_ptr;
};

}
