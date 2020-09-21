#include "sdl/error.h"

#include <fmt/format.h>
#include <SDL2/SDL_error.h>

namespace SDL {

Error::Error() :
	LoggedRuntimeError{"SDL2", SDL_GetError()}
{}

Error::Error(const char* function_name) :
	LoggedRuntimeError{"SDL2", fmt::format("{}: {}", function_name, SDL_GetError())}
{}

}
