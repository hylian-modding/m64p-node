#pragma once

#include <stdexcept>

namespace SDL {

struct Error : std::runtime_error {
	Error();
	Error(const char* function_name);
};

}
