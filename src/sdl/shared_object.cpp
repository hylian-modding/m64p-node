#include "sdl/error.h"
#include "sdl/shared_object.h"

#include <fmt/format.h>
#include <SDL2/SDL_loadso.h>

namespace SDL {

SharedObject::SharedObject() :
	m_ptr{nullptr, SDL_UnloadObject}
{}

SharedObject::SharedObject(const std::filesystem::path& path) :
	SharedObject{}
{
#if defined(__WINDOWS__)
	m_ptr.reset(SDL_LoadObject((path.string() + ".dll").c_str()));
#elif defined(__LINUX__)
	m_ptr.reset(SDL_LoadObject((path.string() + ".so").c_str()));
#elif defined(__MACOSX__)
	m_ptr.reset(SDL_LoadObject((path.string() + ".dylib").c_str()));
#else
#error Platform not supported
#endif

	if (!m_ptr)
		throw Error{"SDL_LoadObject"};
}

void* SharedObject::Get() const
{
    return m_ptr.get();
}

void* SharedObject::LoadFunctionReal(const char* name) const
{
	auto func = SDL_LoadFunction(Get(), name);

	if (!func)
		throw Error{"SDL_LoadFunction"};

	return func;
}

}
