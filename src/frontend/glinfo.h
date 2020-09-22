#pragma once

#include <string>

#include <GL/glew.h>
#include <SDL2/SDL_video.h>

inline const char* GLNameToString(GLenum name)
{
    switch (name) {
    case GL_VENDOR:
        return "Vendor";
    case GL_RENDERER:
        return "Renderer";
    case GL_VERSION:
        return "Version";
    case GL_SHADING_LANGUAGE_VERSION:
        return "Shading language version";
    default:
        return "Unknown";
    }
}

inline const char* GLErrorToString(GLenum error)
{
    switch (error) {
    case GL_INVALID_ENUM:
        return "Invalid enum";
    case GL_INVALID_VALUE:
        return "Invalid value";
    case GL_INVALID_OPERATION:
        return "Invalid operation";
    case GL_STACK_OVERFLOW:
        return "Stack overflow";
    case GL_STACK_UNDERFLOW:
        return "Stack underflow";
    case GL_OUT_OF_MEMORY:
        return "Out of memory";
    default:
        return "Unknown";
    }
}

inline const char* SDLGLProfileToString(int pro)
{
    switch (pro) {
    case SDL_GL_CONTEXT_PROFILE_CORE:
        return "Core";
    case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
        return "Compatibility";
    case SDL_GL_CONTEXT_PROFILE_ES:
        return "ES";
    default:
        return "Unknown";
    }
}

std::string SafeGLGetString(GLenum name);
std::string GetGLContextInfo();
