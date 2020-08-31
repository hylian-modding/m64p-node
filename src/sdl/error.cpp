#include "sdl/error.h"

#include <fmt/format.h>
#include <SDL2/SDL_error.h>

namespace SDL {

Error::Error() :
	std::runtime_error{std::string{"SDL2 error: "} + SDL_GetError()}
{}

Error::Error(const char* function_name) :
	std::runtime_error{fmt::format("SDL2 error: {}: {}", function_name, SDL_GetError())}
{}

}
