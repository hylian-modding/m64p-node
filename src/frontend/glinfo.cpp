#include "common/logger.h"
#include "frontend/glinfo.h"

#include <fmt/format.h>

std::string SafeGLGetString(GLenum name)
{
    auto str = glGetString(name);

    if (!str) {
        GLenum err;
        while (err = glGetError(), err != GL_NO_ERROR)
            Logger::Log(LogCategory::Trace, "OpenGL", fmt::format("Error: {} (0x{:x})", GLErrorToString(err), err));

        auto errfmt = fmt::format("Failed to retrieve {}", GLNameToString(name));
        Logger::Log(LogCategory::Error, "OpenGL", errfmt);
        return errfmt;
    }

    return reinterpret_cast<const char*>(str);
}

std::string GetGLContextInfo()
{
    int maj, min, pro;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &maj);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &min);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &pro);

    return fmt::format("{}.{} {} Profile", maj, min, SDLGLProfileToString(pro));
}
