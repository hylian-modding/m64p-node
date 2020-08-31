#include "frontend/app.h"
#include "frontend/vidext.h"

#include <SDL2/SDL.h>

namespace Frontend {

struct VidExtCallbacks {
    static m64p_error GLSwapBuf()
    {
        Frontend::App::GetInstance().SwapHandler();

        return M64ERR_SUCCESS;
    }
};

}

namespace Frontend::VidExt {

m64p_error ListModes(m64p_2d_size* size_array, int* num_sizes)
{
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);

    size_array->uiWidth = mode.w;
	size_array->uiHeight = mode.h;
	*num_sizes = 1;

    return M64ERR_SUCCESS;
}

m64p_function GLGetProc(const char* proc)
{
    return reinterpret_cast<m64p_function>(SDL_GL_GetProcAddress(proc));
}

m64p_error GLGetAttr(m64p_GLattr attr, int* value)
{
    switch (attr) {
	case M64P_GL_DOUBLEBUFFER:
		SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, value);
		break;
	case M64P_GL_BUFFER_SIZE:
		SDL_GL_GetAttribute(SDL_GL_BUFFER_SIZE, value);
		break;
	case M64P_GL_DEPTH_SIZE:
		SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, value);
		break;
	case M64P_GL_RED_SIZE:
		SDL_GL_GetAttribute(SDL_GL_RED_SIZE, value);
		break;
	case M64P_GL_GREEN_SIZE:
		SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, value);
		break;
	case M64P_GL_BLUE_SIZE:
		SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, value);
		break;
	case M64P_GL_ALPHA_SIZE:
		SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, value);
		break;
	case M64P_GL_SWAP_CONTROL:
		*value = SDL_GL_GetSwapInterval();
		break;
	case M64P_GL_MULTISAMPLEBUFFERS:
		SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, value);
		break;
	case M64P_GL_MULTISAMPLESAMPLES:
		SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, value);
		break;
	case M64P_GL_CONTEXT_MAJOR_VERSION:
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, value);
		break;
	case M64P_GL_CONTEXT_MINOR_VERSION:
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, value);
		break;
	case M64P_GL_CONTEXT_PROFILE_MASK:
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, value);
		switch (*value) {
		case SDL_GL_CONTEXT_PROFILE_CORE:
			*value = M64P_GL_CONTEXT_PROFILE_CORE;
			break;
		case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
			*value = M64P_GL_CONTEXT_PROFILE_COMPATIBILITY;
			break;
		case SDL_GL_CONTEXT_PROFILE_ES:
			*value = M64P_GL_CONTEXT_PROFILE_ES;
			break;
		}
		break;
	}

	return M64ERR_SUCCESS;
}

m64p_error GLSetAttr(m64p_GLattr attr, int value)
{
    switch (attr) {
	case M64P_GL_DOUBLEBUFFER:
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, value);
		break;
	case M64P_GL_BUFFER_SIZE:
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, value);
		break;
	case M64P_GL_DEPTH_SIZE:
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, value);
		break;
	case M64P_GL_RED_SIZE:
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, value);
		break;
	case M64P_GL_GREEN_SIZE:
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, value);
		break;
	case M64P_GL_BLUE_SIZE:
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, value);
		break;
	case M64P_GL_ALPHA_SIZE:
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, value);
		break;
	case M64P_GL_SWAP_CONTROL:
		SDL_GL_SetSwapInterval(value);
		break;
	case M64P_GL_MULTISAMPLEBUFFERS:
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, value);
		break;
	case M64P_GL_MULTISAMPLESAMPLES:
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, value);
		break;
	case M64P_GL_CONTEXT_MAJOR_VERSION:
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, value);
		break;
	case M64P_GL_CONTEXT_MINOR_VERSION:
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, value);
		break;
	case M64P_GL_CONTEXT_PROFILE_MASK:
		switch (value) {
		case M64P_GL_CONTEXT_PROFILE_CORE:
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			break;
		case M64P_GL_CONTEXT_PROFILE_COMPATIBILITY:
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
			break;
		case M64P_GL_CONTEXT_PROFILE_ES:
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
			break;
		}
		break;
	}

	return M64ERR_SUCCESS;
}

void GetFunctions(m64p_video_extension_functions* func)
{
    func->Functions = 14;
    func->VidExtFuncInit = []() { return M64ERR_SUCCESS; };
    func->VidExtFuncQuit = []() { return M64ERR_SUCCESS; };
    func->VidExtFuncListModes = ListModes;
    func->VidExtFuncListRates = [](m64p_2d_size, int*, int*) { return M64ERR_SUCCESS; };
    func->VidExtFuncSetMode = [](int, int, int, int, int) { return M64ERR_SUCCESS; };
    func->VidExtFuncSetModeWithRate = [](int, int, int, int, int, int) { return M64ERR_SUCCESS; };
    func->VidExtFuncGLGetProc = GLGetProc;
    func->VidExtFuncGLSetAttr = GLSetAttr;
    func->VidExtFuncGLGetAttr = GLGetAttr;
    func->VidExtFuncGLSwapBuf = VidExtCallbacks::GLSwapBuf;
    func->VidExtFuncSetCaption = [](const char*) { return M64ERR_SUCCESS; };
    func->VidExtFuncToggleFS = []() { return M64ERR_SUCCESS; };
    func->VidExtFuncResizeWindow = [](int, int) { return M64ERR_SUCCESS; };
    func->VidExtFuncGLGetDefaultFramebuffer = []() { return 0u; };
}

}
