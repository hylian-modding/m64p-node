#pragma once

#include <stdexcept>

#include <m64p_api/m64p_types.h>

namespace M64P {

class Error : public std::runtime_error {
public:
    Error(m64p_error error_code);
	Error(const char* function_name, m64p_error error_code);

private:
    static const char* GetCoreErrorMessage(m64p_error error_code);
};

}
