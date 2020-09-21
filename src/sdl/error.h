#pragma once

#include "common/logged_runtime_error.h"

namespace SDL {

struct Error : LoggedRuntimeError {
	Error();
	Error(const char* function_name);
};

}
